#include "spreadsheet_controller.h"
#include <string>

using namespace CS3505;
using namespace std;

/**
 * Creates a new spreadsheet_controller object with the specified sheet pool.
 */
spreadsheet_controller::spreadsheet_controller(spreadsheet_pool *sheets)
{
  this->sheets = sheets;
}

/**
 * Takes in a properly formatted message struct intended to be sent to the server, 
 * and performs the actions required to handle it. 
 * For example, if it recieves an EDIT, the controller will add those changes to the model, and then
 * call its send_all listener to inform all the clients of the change.
 */
void spreadsheet_controller::handle_message(message msg)
{
  message send; //In most cases, the message we choose to send to the clients.
  switch(msg.type)
    {
    case message_type::CONNECT:
      //Record which spreadsheet this client is currently connected to
      clients.at(msg.identifier).spreadsheet = msg.sheet_name;
      //Get the startup message we want to send to the client.
      send = sheets->get_sheet_contents(msg.sheet_name);
      //Fill in the identifier for the send message
      send.identifier = msg.identifier;
      //Send the startup message to that client
      send_client(msg.identifier,send);
      break;
    case message_type::EDIT:
      //Make the change in the relevent spreadsheet, and get the relevent Change message.
      send = sheets->add_edit(clients.at(msg.identifier).spreadsheet,
				      msg.cell_name, msg.cell_contents);
      //Send the Change message we got to the clients.
      send_all(send);
      break;
    case message_type::UNDO:
      //Make the undo change.
      send = sheets->undo_last_change_on_sheet(
					       clients.at(msg.identifier).spreadsheet);
      if (send.type == message_type::MESSAGE_ERROR)
	{
	  //Log that we didn't return an UNDO, because there was nothing to UNDO.
	  logger::get_logger()->log(string("Didn't execute Undo from ") + msg.identifier +
				    string(", as there was nothing to undo"), loglevel::INFO);
	}
      else
	{
	  //Send the Change message we got to the clients.
	  send_all(send);
	}
      break;
    case message_type::ISTYPING:
      //Record that this client has an ISTYPING request.
      clients.at(msg.identifier).last_is_typing = msg.cell_name;
      //Relay the ISTYPING message to all clients.
      send_all(msg);
      break;
    case message_type::DONETYPING:
      //Record that this cleint satisfied its ISTYPING request.
      clients.at(msg.identifier).last_is_typing = "";
      //Relay the DONETYPING message to all clients.
      send_all(msg);
      break;     
    }
}

/**
 * Registers a client identifier with the listener, so it can properly handle messages
 * pertaining to that client. 
 */
void spreadsheet_controller::register_client(std::string client_identifier)
{
  //Add a struct corresponding to this client to our clients map
  client cli;
  clients.emplace(client_identifier, cli);
}

/**
 * Deregisters a client identifier, so that the controller no longer has to track that client, and so
 * that subsequent register_client calls won't throw an error.
 */
void spreadsheet_controller::deregister_client(std::string client_identifier)
{
  //If this client hasn't sent a DoneTyping message corresponding to its last IsTyping, send the DoneTyping.
  if (clients.at(client_identifier).last_is_typing != "")
    {
      message msg;
      msg.type = message_type::DONETYPING;
      msg.identifier = client_identifier;
      msg.cell_name = clients.at(client_identifier).last_is_typing;
      send_all(msg);
    }
  //Remove the client from our list of clients
  clients.erase(client_identifier);
}


/**
 * Registers with the controller the function it should use when it wants to send all clients a message.
 */
void spreadsheet_controller::register_send_all(std::function<void(message)> func)
{
  this->send_all = func;
}
/**
 * Registers with the controller the function it should use when it wants to send a specific client a message.
 */
void spreadsheet_controller::register_send_client(std::function<void(std::string, message)> func)
{
  this->send_client = func;
}
