//Written by Josh Christensen

#include "server.h"
#include <iostream>

using namespace CS3505;
using namespace std;

/*
 * Launches a spreadsheet server, operating on the RAD protocol, that saves all incoming data,
 * and delivers it to other clients.
 */
int main ()
{
  server our_server;
  //Start the server. Once the server exits, this will return 0 if all is well, and some other number otherwise.
  cout << "Server Starting..." << endl;
  our_server.start();
  //Listen for console commands.
  string command;
  while (our_server.is_running())
    {
      cin >> command;
      //If we recieved a shutdown command.
      if (command == "shutdown")
        {
          cout << "Server shutting down..." << endl;
          if (our_server.stop())
            {
              cout << "Server shutdown gracefully" << endl;
              return 0;
            }
          else
            {
              cout << "Server didn't shutdown gracefully" << endl;
              return 1; //We encountered an error state.
            }
        }
      //Insert possibilities for other commands here.
    }
}


