// $Id: file_sys.cpp,v 1.9 2020-10-26 21:32:08-07 - - $

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <iomanip>

using namespace std;

#include "debug.h"
#include "file_sys.h"

size_t inode::next_inode_nr {1};

ostream& operator<< (ostream& out, file_type type) {
   switch (type) {
      case file_type::PLAIN_TYPE: out << "PLAIN_TYPE"; break;
      case file_type::DIRECTORY_TYPE: out << "DIRECTORY_TYPE"; break;
      default: assert (false);
   };
   return out;
}

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
   //inode_state ctor
   root = inode_ptr(new inode(file_type::DIRECTORY_TYPE));
   cwd = root;

   //Set . and ..
   root->contents->get_dirents().insert(pair<string, inode_ptr>(".", root));
   root->contents->get_dirents().insert(pair<string, inode_ptr>("..", root));
}

inode_ptr inode_state::get_root()	{
   return root;
}

inode_ptr inode_state::get_cwd() {
   return cwd;
}

void inode_state::set_cwd(inode_ptr new_cwd) {
   cwd = new_cwd;
}

file_type inode::type() const  {
   return contents->type();
}

const string& inode_state::prompt() const { return prompt_; }

void inode_state::prompt (const string& prompt) {
   prompt_ = prompt;
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
      default: assert (false);
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

base_file_ptr inode::get_contents() {
   return contents;
}

size_t inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}

int inode::size() {
   return contents->size();
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}

map<string, inode_ptr>& base_file::get_dirents() {
   throw file_error ("is a " + error_file_type());
}

file_type base_file::type() const {
   throw file_error ("basefile type() error");
}

file_type plain_file::type() const {
   return file_type::PLAIN_TYPE;
}

size_t plain_file::size() const {
   int size = data.size() - 1;
   for(string word : data) {
      size = size + word.length();
   }
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   data = words;
}

file_type directory::type() const {
   return file_type::DIRECTORY_TYPE;
}

size_t directory::size() const {
   size_t size = dirents.size();
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr remove_node = nullptr;
   //Check if filename exists
   map<string, inode_ptr>::iterator iter = dirents.find(filename);
   if (iter != dirents.end()) {
      remove_node = dirents.at(filename);
      if(error_file_type() == "directory") {
         if(remove_node->get_contents()->size() >= 3){
            throw file_error(filename + " is not empty");
         }else {
            remove_node->get_contents()->get_dirents().clear();
         }
      }
      dirents.erase(filename);
   }else {
      throw file_error(filename + " could not be found");
   }
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   inode_ptr dir_ptr = nullptr;
   //Check if dirname exists
   map<string, inode_ptr>::iterator iter = dirents.find(dirname);
   if (iter != dirents.end()) {
      throw file_error(dirname + " is an existing file/directory");
   }else {
      //Initialize inode
      inode dir(file_type::DIRECTORY_TYPE);
      dir_ptr = make_shared<inode>(dir);
      inode_ptr dir_cur = dirents.at(".");
      //Update dirents for cwd and created dir
      dirents.insert(pair<string, inode_ptr>(dirname, dir_ptr));
      dir_ptr->get_contents()->get_dirents().insert(pair<string, inode_ptr>(".", dir_ptr));
      dir_ptr->get_contents()->get_dirents().insert(pair<string, inode_ptr>("..", dir_cur));
   }
   return dir_ptr;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   inode_ptr file_ptr = nullptr;

   //Check if filename exists
   map<string, inode_ptr>::iterator iter = dirents.find(filename);
   if (iter != dirents.end()) {
      throw file_error(filename + " is an existing file/directory");
   }else {
      inode file(file_type::PLAIN_TYPE);
      file_ptr = make_shared<inode>(file);
      dirents.insert(pair<string, inode_ptr>(filename, file_ptr));
   }
   return file_ptr;
}

map<string, inode_ptr>& directory::get_dirents() {
   return dirents;
}

ostream& operator<< (ostream& out, const base_file&) {
   out << "base_file ostream error";
   return out;
}

ostream& operator<< (ostream& out, const plain_file& file) {
   out << file.readfile();
   return out;
}

ostream& operator<< (ostream& out, const inode& inode) {
   if (inode.type() == file_type::DIRECTORY_TYPE) {
      out << *dynamic_pointer_cast<directory>(inode.contents);
   } else {
      out << *dynamic_pointer_cast<plain_file>(inode.contents);
   }
   return out;
}

ostream& operator<< (ostream& out, const directory& dir) {
   for(map<string, inode_ptr>::const_iterator iter = dir.dirents.begin(); iter != dir.dirents.end(); iter++) {
      out << setw(5) << iter->second->get_inode_nr();
      out << setw(5) << iter->second->size();
      out << setw(10) << iter->first; 
      if (iter->second->type() == file_type::DIRECTORY_TYPE) {
         cout << "/";
      }
      cout << endl;
   }
   return out;
}

