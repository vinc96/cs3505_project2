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
  //Parse the message and send it to our controller.
  

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
  
  callbacks.client_connected = std::bind(&server::client_connected, this, std::placeholders::_1);
  callbacks.message_received = std::bind(&server::message_received, this, std::placeholders::_1,
					 std::placeholders::_2);
  callbacks.client_disconnected = std::bind(&server::client_disconnected, this, std::placeholders::_1);
  
  networking = new socket_manager(callbacks);
  //Create the pool of spreadsheets
  spreadsheets = new spreadsheet_pool();
  //Instantiate the controller, and provide it callbacks to networking, and vice versa.
  controller = new spreadsheet_controller(spreadsheets);
  //Log that the server started
  log->log("Server Started", loglevel::INFO);
  /*
  //Spam "Test i" to all sockets
  for (int i = 0; i < 2000000000; i++)
    {
      networking->send_all("TEST");
    }
  */
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
