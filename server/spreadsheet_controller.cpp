#include "spreadsheet_controller.h"

using namespace CS3505;

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
  switch(msg.type)
    {
    case message_type::CONNECT:
      //PLACEHOLDER: SEND A STARTUP WITH A SINGLE CELL AS "YOU GOT IT!, ID of 0."
      msg.type = message_type::STARTUP;
      msg.cells.emplace("A1", "YOU GOT IT!");
      send_client(msg.identifier, msg);
      break;
    case message_type::EDIT:
      break;
    case message_type::UNDO:
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
