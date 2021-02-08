// $Id: commands.cpp,v 1.20 2021-01-11 15:52:17-08 - - $

#include "commands.h"
#include "debug.h"

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
   {"#"     , fn_comment},
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

inode_ptr validate_path(inode_state &state, wordvec path)	{
   inode_ptr wd = nullptr;
   //Determine if path starts from root or cwd
   if(path.front() == "/") {
      return state.get_root();
   }
   if(path.front()[0] == '/') {
      wd = state.get_root();
   }else {
      wd = state.get_cwd();
   }
   wordvec path_split = split(path.front(), "/");
   for(wordvec::iterator iter = path_split.begin(); iter != path_split.end(); iter++) {
      //*it represents dir/file
      map<string, inode_ptr> dirents = wd->get_contents()->get_dirents();
      map<string, inode_ptr>::iterator it = dirents.find(*iter);
      if(it != dirents.end()) {
         wd = dirents.at(*iter);
      }else {
         throw command_error("not a valid path");
      }
   }
   return wd;
}

bool check_path(inode_state &state, wordvec &path, bool from_root) {
   inode_ptr wd = nullptr;
   if(from_root) {
      wd = state.get_root();
   }else	{
      wd = state.get_cwd();
   }
   for(wordvec::iterator iter = path.begin(); iter != path.end(); iter++) {
      //*it represents dir/file
      map<string, inode_ptr> dirents = wd->get_contents()->get_dirents();
      map<string, inode_ptr>::iterator it = dirents.find(*iter);
      if(it != dirents.end()) {
         wd = dirents.at(*iter);
      }else {
         return false;
      }
   }
   return true;
}
void fn_comment (inode_state& state, const wordvec& words) {
   auto root = state.get_root();
   auto comment = words.at(1);
}

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size() < 2) {
      throw command_error("cat: too few arguments");
   }else {
      inode_ptr file = nullptr;
      wordvec tmp;
      for(unsigned int i = 1; i < words.size(); i++) {
         tmp.push_back(words.at(i));
         file = validate_path(state, tmp);
         if (file->get_contents()->type() == file_type::PLAIN_TYPE) {   
            cout << file->get_contents()->readfile() << endl;
            tmp.clear();
         }else {
            throw command_error(words.at(i) + " is a directory not a file");
         }
      }
   }
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size() > 2) {
      throw command_error("cd: too many arguments");
   }else if (words.size() == 2){
      wordvec tmp;
      tmp.push_back(words.at(1));
      inode_ptr dir = validate_path(state, tmp);
      if(dir->get_contents()->type() == file_type::DIRECTORY_TYPE) {
         state.set_cwd(dir);
      }else {
         throw command_error(words.at(1) + " is a file not a directory");
      }
   }else {
      state.set_cwd(state.get_root());
   }
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   int status_ = 0;
   if (words.size() > 1) {
      for (auto letter : words.at(1)) {
         if('0' > letter or letter > '9' ) {
            status_ = 127;
            break; 
         }
      }
      if(!status_) {
         status_ = stoi(words.at(1));  
      }
   }
   exec::status(status_);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() == 2) {
     wordvec tmp;
     tmp.push_back(words.at(1));
     if(words.at(1) == "/") {
         cout << "/:\n" << *state.get_root() << endl;
     }else if(words.at(1) == ".") {
        cout << ".:\n" << *state.get_cwd() << endl;
     }else if(words.at(1) == "..") {
        cout << "..:\n" << *state.get_cwd()->get_contents()->get_dirents().at("..") << endl;
     }else {
        inode_ptr dir = validate_path(state, tmp);
        string dir_name = words.at(1);
        cout << dir_name + ":\n" <<*dir << endl;
     }
   }else if (words.size() == 1) {
      if (state.get_root() == state.get_cwd()) {
         cout << "/:" << endl;
      }else {
         inode_ptr parent = state.get_cwd()->get_contents()->get_dirents().at("..");
         auto parent_dir = parent->get_contents()->get_dirents();

         for(map<string, inode_ptr>::iterator iter = parent_dir.begin(); iter != parent_dir.end(); iter++) {
            if(iter->second == state.get_cwd()) {
               cout << iter->first + ":" << endl;
            }
         }
      }
      cout << *state.get_cwd() << endl;
   }else {
      throw command_error("ls: too many arguments");
   }
}

void recursive_ls(inode_state& state, inode_ptr inode, inode_ptr root) {
   if(inode == state.get_root()) {
      cout << "/:" << endl;  
   }else {
      wordvec path;
      inode_ptr name = inode;
      while (name != root->get_contents()->get_dirents().at("..")) {
         inode_ptr parent = name->get_contents()->get_dirents().at("..");
         auto parent_dir = parent->get_contents()->get_dirents();
         for(map<string, inode_ptr>::iterator iter = parent_dir.begin(); iter != parent_dir.end(); iter++) {
            if(iter->second == name) {
               path.insert(path.begin(), iter->first);
            }
         }
         name = parent;
      }

      for(wordvec::iterator iter = path.begin(); iter != path.end(); iter++) {
          cout << "/" + *iter;
      }
      cout << ":" << endl;
   }
   cout << *inode << endl;

   auto dir = inode->get_contents()->get_dirents();

   for(map<string, inode_ptr>::iterator iter = dir.begin(); iter != dir.end(); iter++) {
      if (iter->first != "." and iter->first != "..") {
         if (iter->second->type() == file_type::DIRECTORY_TYPE) {
            recursive_ls(state, iter->second, root);
         }
      }
   }

}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   wordvec tmp;
   if (words.size() == 2) {
     tmp.push_back(words.at(1));
     inode_ptr dir = validate_path(state, tmp);
     recursive_ls(state, dir, dir);
   }else if (words.size() == 1) {
      recursive_ls(state, state.get_cwd(), state.get_cwd());
   }else {
      for(unsigned int i = 1; i < words.size(); i++) {
         tmp.push_back(words.at(i));
         inode_ptr dir = validate_path(state, tmp);
         recursive_ls(state, dir, dir);
         tmp.clear();
      }
   }   
}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() == 1) {
      throw command_error("make: requires argument");
   }else if (words.size() >= 2) {
      wordvec path = split(words.at(1), "/");
      path.pop_back();
      if(check_path(state, path, words.at(1)[0] == '/')) {
         inode_ptr wd = validate_path(state, join(path, words.at(1)[0] == '/'));
         inode_ptr file = wd->get_contents()->mkfile(split(words.at(1), "/").back());
         wordvec write_data;
         for(unsigned int i = 2; i < words.size(); i++) {
            write_data.push_back(words.at(i));
         }
         file->get_contents()->writefile(write_data);
      }else {
         throw command_error("mkdir: not a valid path");
      }
   }else {
      throw command_error("make: too many arguments");
   }
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() == 2) {
      wordvec path = split(words.at(1), "/");  
      path.pop_back();
      if(check_path(state, path, words.at(1)[0] == '/')) {
         inode_ptr wd = validate_path(state, join(path, words.at(1)[0] == '/'));
         wd->get_contents()->mkdir(split(words.at(1), "/").back());
      }else {
         throw command_error("mkdir: not a valid path");
      }
   }else {
      throw command_error("mkdir: only accepts one argument");
   }
}

void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() < 2) {
      throw command_error("prompt: too few arguments");
   }else {
      string _prompt = "";
      for(wordvec::const_iterator iter = words.begin() + 1; iter != words.end(); iter++) {
         _prompt = _prompt + *iter + " ";
      }
      state.prompt(_prompt);
   }
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   cout << "/";
   map<string, inode_ptr> dir = state.get_cwd()->get_contents()->get_dirents();
   if(dir.at(".") != dir.at("..")) {
      inode_ptr parent = dir.at("..");
      map<string, inode_ptr> parent_dir = parent->get_contents()->get_dirents();
      for(map<string, inode_ptr>::iterator iter = parent_dir.begin(); iter != parent_dir.end(); iter++) { 
         if(iter->second == dir.at(".")) {
            cout << iter->first;
         }
      } 
   }
   cout << endl;
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if(words.size() < 2) {
      throw command_error("rm: too few arguments");
   }else {
      wordvec path = split(words.at(1), "/");
      string name = path.back();
      path.pop_back();
      if(check_path(state, path, words.at(1)[0] == '/')) { 
         inode_ptr wd = validate_path(state, join(path, words.at(1)[0] == '/'));
         map<string, inode_ptr>::iterator it = wd->get_contents()->get_dirents().find(name);
         if (it == wd->get_contents()->get_dirents().end()) {
            throw ("rm: could not find " + name);
         }else {
            inode_ptr target = wd->get_contents()->get_dirents().at(name);
            if(target->type() == file_type::DIRECTORY_TYPE) {
               wd->get_contents()->remove(name);
            }else {
               wd->get_contents()->get_dirents().erase(name);
            }
         }   
      }
   }
}

void recursive_rm(inode_ptr inode) {
   auto dir = inode->get_contents()->get_dirents();
   for(map<string, inode_ptr>::iterator iter = dir.begin(); iter != dir.end(); iter++) {
      if (iter->first != "." and iter->first != "..") {
         if (iter->second->type() == file_type::DIRECTORY_TYPE) {
            recursive_rm(iter->second);
            iter->second->get_contents()->get_dirents().clear();
         }
      }
   }
   dir.clear();
}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() != 2) {
      throw command_error("rmr: only accepts 1 argument");
   }else {
      wordvec path = split(words.at(1), "/");
      string name = path.back();
      path.pop_back();
      if(check_path(state, path, words.at(1)[0] == '/')) {
         inode_ptr wd = validate_path(state, join(path, words.at(1)[0] == '/'));
         map<string, inode_ptr>::iterator it = wd->get_contents()->get_dirents().find(name);
         if (it == wd->get_contents()->get_dirents().end()) {
            throw ("rmr: could not find " + name);
         }else {
            inode_ptr target = wd->get_contents()->get_dirents().at(name);
            if(target->type() == file_type::DIRECTORY_TYPE) {
               recursive_rm(target);
               wd->get_contents()->get_dirents().erase(name);
            }else {
               throw command_error("rmr: argument must be directory");
            }
         }
      }
   }
}

