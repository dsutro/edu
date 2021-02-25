// $Id: main.cpp,v 1.13 2021-02-01 18:58:18-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <unistd.h>
#include <regex>
#include <fstream>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}


void sanitize(string& line) {
   string::iterator range;
   for (auto iter = line.begin(); iter != line.end(); iter++) {
      if (isspace(*iter)) {
         continue;
      }else {
         if(iter != line.begin()) {
            range = iter;
            line.erase(line.begin(), range);
         }
         break;
      }
   }
   for (auto iter = line.end()-1; iter != line.begin(); iter--) {
      if (isspace(*iter)) {
         continue;
      }else {
         if(iter != line.end()-1) {
            range = iter+1;
            line.erase(range, line.end()-1);
         }
         break;
      }
   }
}


int main(int argc, char** argv) {
   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   str_str_map map;
   string line;
   string fname;
   int line_num;

   //Regular Expression
   smatch result;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   

   if(argc < 2) {
      //No File Provided
      fname = "-";
      line_num = 0;
      while(getline(cin, line)) {
         if (cin.eof()) break;
         line_num++;
         sanitize(line);
         cout << fname << ": " << line_num << ": " << line << endl;
         if (regex_search (line, result, comment_regex)) {
            continue;
         }
         if (regex_search (line, result, key_value_regex)) {
            auto key = result[1];
            auto value = result[2];
            if(key != "" and value != "") {
               cout << key << " = " << value << endl;
               map.insert(str_str_pair(key, value));
            }else if (key != "" and value == "") {
               auto search = map.find(key);
               if(search != map.end()) {
                  map.erase(search);
               }else {
                  cout << key << ": key not found" << endl;
               }
            }else if (key == "" and value != "") {
               for(auto iter = map.begin(); iter != map.end(); ++iter) {
                  if(iter->second == value) {
                     cout << iter->first << " = " << value << endl;
                  }
               }
            }else if (key == "" and value == "") {
               for (auto iter = map.begin(); 
                    iter != map.end(); ++iter) {
                  cout << iter->first << " = " << iter->second << endl;
               }
            }else {
               complain();
            }
         }else if (regex_search (line, result, trimmed_regex)) {
            auto key = result[1];
            auto search = map.find(key);
            if(search != map.end()) {
               cout << key << " = " << search->second << endl;
            }else {
               cout << key << ": key not found" << endl;
            }
         } 
      }
   }else {
      for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
         fname = *argp;
         line_num = 0;
         if (fname == "-") {
            //stdin
            while(getline(cin, line)) {
               if (cin.eof()) break;
               line_num++;
               sanitize(line);
               cout << fname << ": " << line_num << 
               ": " << line << endl;
               if (regex_search (line, result, comment_regex)) {
                  continue;
               }
               if (regex_search (line, result, key_value_regex)) {
                  auto key = result[1];
                  auto value = result[2];
                  if(key != "" and value != "") {
                     cout << key << " = " << value << endl;
                     str_str_pair pair(result[1], result[2]);
                     map.insert(pair);
                  }else if (key != "" and value == "") {
                     auto search = map.find(key);
                     if(search != map.end()) {
                        map.erase(search);
                     }else {
                        cout << key << ": key not found" << endl;
                     }
                  }else if (key == "" and value != "") {
                     for(auto iter = map.begin(); 
                              iter != map.end(); ++iter) {
                        if(iter->second == value) {
                           cout << iter->first << " = " << 
                                   value << endl;
                        }
                     }
                  }else if (key == "" and value == "") {
                     for (auto iter = map.begin(); 
                               iter != map.end(); ++iter) {
                        cout << iter->first << " = " << 
                                iter->second << endl;
                     }
                  }else {
                     complain();
                  }
               }else if (regex_search (line, result, trimmed_regex)) {
                  auto key = result[1];
                  auto search = map.find(key);
                  if(search != map.end()) {
                     cout << key << " = " << search->second << endl;
                  }else {
                     cout << key << ": key not found" << endl;
                  }
               }
            }
         }else {
            //file
            ifstream infile (fname);
            if (infile.fail()) {
               complain() << fname << ": " << 
               "No such file or directory" << endl;
            }else {
               while(getline(infile, line)) {
                  if (infile.eof()) break;
                  line_num++;
                  sanitize(line);
                  cout << fname << ": " << line_num << 
                                   ": " << line << endl;
                  if (regex_search (line, result, comment_regex)) {
                     continue;
                  }
                  if (regex_search (line, result, key_value_regex)) {
                     auto key = result[1];
                     auto value = result[2];
                     if(key != "" and value != "") {
                        cout << key << " = " << value << endl;
                        str_str_pair pair(result[1], result[2]);
                        map.insert(pair);
                     }else if (key != "" and value == "") {
                        auto search = map.find(key);
                        if(search != map.end()) {
                           map.erase(search);
                        }else {
                           cout << key << ": key not found" << endl;
                        }
                     }else if (key == "" and value != "") {
                        for(auto iter = map.begin(); 
                                 iter != map.end(); ++iter) {
                           if(iter->second == value) {
                              cout << iter->first << " = " << 
                              value << endl;
                           }
                        }
                     }else if (key == "" and value == "") {
                        for (auto iter = map.begin(); 
                                  iter != map.end(); ++iter) {
                           cout << iter->first << " = " << 
                                   iter->second << endl;
                        }
                     }else {
                        complain();
                     }
                  }else if (regex_search (line, 
                            result, trimmed_regex)) {
                     auto key = result[1];
                     auto search = map.find(key);
                     if(search != map.end()) {
                        cout << key << " = " << search->second << endl;
                     }else {
                        cout << key << ": key not found" << endl;
                     }
                  }
               } 
               infile.close();
            }
         }
      }
   }
   return sys_info::exit_status();
}


