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
      msg.identifier = "0";
      msg.cells.emplace("A1", "YOU GOT IT!");
      send_client(msg.identifier, msg);
      break;
    case message_type::EDIT:
      break;
    case message_type::UNDO:
      break;
    case message_type::ISTYPING:
      //Simply relay the ISTYPING message to all clients.
      send_all(msg);
      break;
    case message_type::DONETYPING:
      //Simply relay the DONETYPING message to all clients.
      send_all(msg);
      break;

      
    }
}

/**
 * Registers a client identifier with the listener, so it can properly handle messages
 * pertaining to that client. 
 */
void register_client(std::string client_identifier)
{

}

/**
 * Deregisters a client identifier, so that the controller no longer has to track that client, and so
 * that subsequent register_client calls won't throw an error.
 */
void deregister_client(std::string client_identifier)
{

}
