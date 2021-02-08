#include <iostream>
#include <string>

#include "file_sys.h"
#include "util.h"
#include "commands.h"

int main(int, char**)	{
	cout << "Hello World" << endl;
	inode_state *state = new inode_state();
	
  cout << state->prompt() << endl;

  cout << *state << endl;
 
	return 0;
}
