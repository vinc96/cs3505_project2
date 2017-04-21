#include "spreadsheet_controller.h"

using namespace CS3505;

/**
 * Creates a new spreadsheet_controller object with the specified sheet pool.
 */
spreadsheet_controller::spreadsheet_controller(spreadsheet_pool sheet)
{

}

/**
 * Takes in a properly formatted message struct, and performs the actions required to handle it. 
 * For example, if it recieves an EDIT, the controller will add those changes to the model, and then
 * call its send_all listener to inform all the clients of the change.
 */
void spreadsheet_controller::handle_message(message &msg)
{
  
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
