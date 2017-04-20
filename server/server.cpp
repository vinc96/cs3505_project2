//Written by Josh Christensen

#include "server.h"
#include "logger.h"

using namespace CS3505;
using namespace std;

/*
 * The function that's called when a new client connects on the socket. Should handle sending
 * initialization data to the client.
 *
 * Parameters:
 * spreadsheet_name: The name of the spreadsheet the client wants to connect to.
 * client_identifier: The identifier that the socket_manager has assigned to this specific client.
 *  Used as a parameter in send_message to send data to a specific client.
 */
void server::client_connected(std::string client_identifier)
{

}
/*
 * The function that's called when we recieve a complete, terminated message from some client.
 *
 * Parameters:
 * client_identifier: the client identifier of the client that disconnected.
 * message: The message recieved from the client.
 */
void server::message_received(std::string client_identifier, std::string message)
{

}
/*
 * The function that's called when a client's socket connection is lost (either through
 * disconnection, or network issues).
 *
 * Parameters:
 * client_identifier: the client identifier of the client that disconnected.
 */
void server::client_disconnected(std::string client_identifier)
{

}

  

bool server::start()
{
  //Create a new logger:
  logger *log = logger::make_logger("log", loglevel::ALL, loglevel::ALL);
  if (log == NULL)
    {
      std::cout << "[ERROR]: LOGGER FAILED TO START." << std::endl;
      return false;
    }

  //Create the socket_manager
  network_callbacks callbacks;
  /*
  callbacks.client_connected = bind(&server::client_connected, this, placeholders::_1);
  callbacks.message_received = bind(&server::message_received, this, placeholders::_1,
				    placeholders::_2);
  callbacks.client_disconnected = bind(&server::client_disconnected, this, placeholders::_1);
  */
  networking = new socket_manager(callbacks);
  //Insert other initialization code here.
  
  //Log that the server started
  log->log("Server Started", loglevel::INFO);
    
  return true; 
}

bool server::stop()
{
  //Clean things up.
  //Close our socket manager.
  delete(networking);
  
  return true; //Placeholder
}

bool server::is_running()
{
  return true; //Placeholder
}
