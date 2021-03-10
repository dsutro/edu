// $Id: cxi.cpp,v 1.1 2020-11-22 16:51:43-08 - - $

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

#include <libgen.h>
#include <sys/types.h>
#include <unistd.h>

#include "protocol.h"
#include "logstream.h"
#include "sockets.h"

logstream outlog (cout);
struct cxi_exit: public exception {};

unordered_map<string,cxi_command> command_map {
   {"exit", cxi_command::EXIT},
   {"help", cxi_command::HELP},
   {"ls"  , cxi_command::LS  },
   {"put" , cxi_command::PUT},
   {"rm"  , cxi_command::RM},
   {"get" , cxi_command::GET},
};

static const char help[] = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cxi_help() {
   cout << help;
}

void cxi_ls (client_socket& server) {
   cxi_header header;
   header.command = cxi_command::LS;
   outlog << "sending header " << header << endl;
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   outlog << "received header " << header << endl;
   if (header.command != cxi_command::LSOUT) {
      outlog << "sent LS, server did not return LSOUT" << endl;
      outlog << "server returned " << header << endl;
   }else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      outlog << "received " << host_nbytes << " bytes" << endl;
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}

void cxi_put(client_socket& server, string filename) {
   if (filename.size() > FILENAME_SIZE) {
      outlog << "cxi_put: FILE ERROR: " << filename << " is too large" << endl;
      return; 
   }
   cxi_header header;
   header.command = cxi_command::PUT;
   strcpy(header.filename, filename.c_str());
   outlog << "cxi_put: " << header.filename << endl;
   ifstream file_stream(filename, ifstream::binary);

   if (file_stream) {
      file_stream.seekg(0, file_stream.end);
      int size = file_stream.tellg();
      char* file_data = new char[size];
      file_stream.seekg(0, file_stream.beg);

      file_stream.read(file_data, size); 
      header.nbytes = size;
      outlog << "sending header " << header << endl;
      outlog << "file data: " << file_data << endl << "file size: " << size << endl;
      send_packet(server, &header, sizeof header);
      send_packet(server, file_data, size);
      recv_packet(server, &header, sizeof header);

      outlog << "receiving header " << header << endl;

      if (header.command == cxi_command::ACK) {
         outlog << "PUT successfull, " << filename << " has been added to the server directory." << endl;
      }else {
         outlog << "PUT unsuccessfull." << endl;
      } 
   }else {
      outlog << "cxi_put: FILE ERROR: " << filename << " could not be opened" << endl;
   }
   file_stream.close();
}

void cxi_get(client_socket& server, string filename) {
   if (filename.size() > FILENAME_SIZE) {
      outlog << "cxi_get: FILE ERROR: " << filename << " is too large" << endl;
      return;
   }
   cxi_header header;
   header.command = cxi_command::GET;
   strcpy(header.filename, filename.c_str());
   outlog << "cxi_get: " << header.filename << endl;

   outlog << "sending header " << header << endl;
   send_packet(server, &header, sizeof header);
   recv_packet(server, &header, sizeof header);
   outlog << "receiving header " << header << endl;

   if(header.command == cxi_command::FILEOUT) {
      char* file_data = new char[header.nbytes+1];
      recv_packet (server, file_data, header.nbytes);
      outlog << "received header " << header << endl;
      file_data[header.nbytes] = '\0';
     
      ofstream file_stream(header.filename, ifstream::binary);
      file_stream.write(file_data, header.nbytes);
      file_stream.close();
   }else {
      outlog << "cxi_get: FILE ERROR: could not get " << filename << endl;
   }
}

void cxi_rm(client_socket& server, string filename) {
   if (filename.size() > FILENAME_SIZE) {
      outlog << "cxi_put: FILE ERROR: " << filename << " is too large" << endl;
      return;
   }
   cxi_header header;
   header.command = cxi_command::RM;
   strcpy(header.filename, filename.c_str());
   header.nbytes = 0;

   outlog << "sending header " << header << endl;
   send_packet(server, &header, sizeof header);
   recv_packet(server, &header, sizeof header);
   outlog << "received header " << header << endl;
   
   if (header.command == cxi_command::ACK) {
      outlog << "cxi_rm: File removed successfully." << endl;
   }else if (header.command == cxi_command::NAK) {
      outlog << "cxi_rm: Failed to remove file." << endl;
   }else {
      outlog << "Unexpected header command received." << endl;
   }
}

void usage() {
   cerr << "Usage: " << outlog.execname() << " [host] [port]" << endl;
   throw cxi_exit();
}

int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << "starting" << endl;
   vector<string> args (&argv[1], &argv[argc]);
   if (args.size() > 2) usage();
   string host = get_cxi_server_host (args, 0);
   in_port_t port = get_cxi_server_port (args, 1);
   outlog << to_string (hostinfo()) << endl;
   try {
      outlog << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      outlog << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cxi_exit();
         outlog << "command " << line << endl;
         string fname = "";
         string command = line;
         size_t index = line.find(" ");
         if (index != string::npos) {
            fname = line.substr(index+1);
            command = line.substr(0, index);
          }

         const auto& itor = command_map.find (command);
         cxi_command cmd = itor == command_map.end()
                         ? cxi_command::ERROR : itor->second;

         switch (cmd) {
            case cxi_command::EXIT:
               throw cxi_exit();
               break;
            case cxi_command::HELP:
               cxi_help();
               break;
            case cxi_command::LS:
               cxi_ls (server);
               break;
            case cxi_command::PUT:
               cxi_put (server, fname);
               break;
            case cxi_command::GET:
               cxi_get (server, fname);
               break;
            case cxi_command::RM:
               cxi_rm (server, fname);
               break;
            default:
               outlog << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cxi_exit& error) {
      outlog << "caught cxi_exit" << endl;
   }
   outlog << "finishing" << endl;
   return 0;
}
	
