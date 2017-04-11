//Written by Josh Christensen

#include "socket_manager.h"

using namespace CS3505;
using namespace std;


/*
 * Description:
 * Creates a new socket_manager class, logging to the specified file.
 *
 * Parameters:
 * callbacks: The network_callbacks struct we should use for our callbacks.
 * log_file: A string, corresponding to some relative filepath that we should log our output to.
 * logged output is plaintext output, logged to the end of the file.
 */
socket_manager::socket_manager(network_callbacks callbacks, string log_file)
  : callbacks(callbacks)
{

}

/*
 * Description: The destructor for the socket_manager class. Cleans up all used system resources.
 */
socket_manager::~socket_manager()
{

}

/*
 * Sends the specified message string to the specified client identifier. If that client identifier
 * corresponds to a currently connected client, sends the message and returns true. Otherwise,
 * returns false.
 */
bool socket_manager::send_message(string message, string client_identifier)
{

}

/*
 * Sends a message to all connected clients. Returns true if there's at least one client. false otherwise.
 */
bool socket_manager::send_all(string message)
{

}
