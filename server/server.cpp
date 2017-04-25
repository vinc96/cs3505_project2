//Written by Josh Christensen (u0978248)

#include "server.h"
#include "logger.h"
#include "message_parser.h"

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
  controller->register_client(client_identifier);
}

/*
 * The function that's called when we recieve a complete, terminated message from some client.
 *
 * Parameters:
 * client_identifier: the client identifier of the client that disconnected.
 * message: The message recieved from the client.
 */
void server::message_received(std::string client_identifier, std::string message_str)
{
  //Grab the logger.
  logger *log = logger::get_logger();
  //Parse the message and send it to our controller, as long as we didn't log an error.
  message parsed_msg;
  parsed_msg = message_parser::parse_client_message(message_str, client_identifier);
  if (parsed_msg.type == message_type::MESSAGE_ERROR)
    {
      log->log(string("Cannot parse message: ") + message_str, loglevel::WARNING);
      log->log(string("Kicking client: ") + client_identifier, loglevel::WARNING);
      networking->kick_client(client_identifier);
    }
  else if (parsed_msg.type == message_type::ISTYPING || parsed_msg.type == message_type::DONETYPING)
    {
      //If we have a mismatched client identifier, kick the client from the server.
      if (parsed_msg.identifier != client_identifier)
	{
	  log->log(string("Mismatched Client identifier: ") + message_str, loglevel::WARNING);
	  log->log(string("Kicking client: ") + client_identifier, loglevel::WARNING);
	  networking->kick_client(client_identifier);
	}
      else
	{
	  if (!controller->handle_message(parsed_msg))
	    {
	      //If we were told to kick the client, kick the client.
	      log->log(string("Illegal message: ") + message_str, loglevel::WARNING);
	      log->log(string("Kicking client: ") + client_identifier, loglevel::WARNING);
	      networking->kick_client(client_identifier);
	    }
	}
    }
  else
    {
      if (!controller->handle_message(parsed_msg))
	{
	  //If we were told to kick the client, kick the client.
	  log->log(string("Illegal message: ") + message_str, loglevel::WARNING);
	  log->log(string("Kicking client: ") + client_identifier, loglevel::WARNING);
	  networking->kick_client(client_identifier);
	}
    }
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
  controller->deregister_client(client_identifier);
}

  

bool server::start()
{
  //Create a new logger:
  logger *log = logger::make_logger("log", loglevel::ALL, loglevel::ALL);
  if (log == NULL)
    {
      std::cout << "[ERROR]: LOGGER FAILED TO START. SERVER EXITING." << std::endl;
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
  controller->register_send_all([=](message msg){
      string str_msg = message_parser::encode_client_message(msg);
      if (!networking->send_all(str_msg))
	{
	  logger::get_logger()->log(string("Message Failed to Send: ") + str_msg,
				    loglevel::WARNING);
	}
      else
	{
	  logger::get_logger()->log(string("Message Sent: ") + str_msg,
				    loglevel::ALL);
	}
    });
  controller->register_send_client([=](string ident, message msg){
      string str_msg = message_parser::encode_client_message(msg);
      if (!networking->send_message(str_msg, ident))
	{
	  logger::get_logger()->log(string("Message Failed to Send to: ") + ident +
				    string(" Message: ") + str_msg, loglevel::WARNING);
	}
      else
	{
	  logger::get_logger()->log(string("Message Sent to: ") + ident +
				    string(" Message: ") + str_msg, loglevel::ALL);
	}
    });
  //Log that the server started
  log->log("Server Started", loglevel::INFO);
  return true; 
}

bool server::stop()
{
  //Clean things up.
  //Close our socket manager, controller and spreadsheets.
  delete(networking);
  delete(controller);
  delete(spreadsheets);
  return true;
}

bool server::is_running()
{
  return true;
}
