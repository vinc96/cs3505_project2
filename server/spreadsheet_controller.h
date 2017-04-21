#ifndef SPREADSHEET_CONTROLLER_H
#define SPREADSHEET_CONTROLLER_H

#include <string>
#include <unordered_map>
#include <functional>
#include "message_parser.h"
#include "spreadsheet_pool.h"

namespace CS3505
{
/**
 * Represents a controller object that responds to messages recieved from the client.
 */
class spreadsheet_controller
{
private:
/**
 * This controler's pool of spreadsheets it can access.
 */
spreadsheet_pool sheets;

/**
 * A map of all currently connected to clients, mapped to the spreadsheet name that they're 
 * currently editing.
 */
//std::unordered_map<std::string client_identifier, std::string sheet_name> clients;
/**
 * The method that this controller calls in order to send a message struct out to all the clients.
 */
std::function<void(message)> send_all;
/**
 * The method that this controller calls in order to send a message struct to a specific client.
 */
std::function<void(std::string, message)> send_client;
public:

/**
 * Creates a new spreadsheet_controller object with the specified sheet pool.
 */
spreadsheet_controller(spreadsheet_pool sheet);

/**
 * Takes in a properly formatted message struct, and performs the actions required to handle it. 
 * For example, if it recieves an EDIT, the controller will add those changes to the model, and then
 * call its send_all listener to inform all the clients of the change.
 */
void handle_message(message &msg);
/**
 * Registers a client identifier with the listener, so it can properly handle messages
 * pertaining to that client. 
 */
void register_client(std::string client_identifier);
/**
 * Deregisters a client identifier, so that the controller no longer has to track that client, and so
 * that subsequent register_client calls won't throw an error.
 */
void deregister_client(std::string client_identifier);
};
}

#endif
