#ifndef SPREADSHEET_CONTROLLER_H
#define SPREADSHEET_CONTROLLER_H

#include <string>
#include <unordered_map>
#include <functional>
#include "spreadsheet_pool.h"
#include "message_parser.h"

namespace CS3505
{
  struct client
  {
    //The last IsTyping message we received without an acompanying DoneTyping.
    std::string last_is_typing;
    //The spreadsheet the client is currently connected to.
    std::string spreadsheet = "";
  };
  /**
   * Represents a controller object that responds to messages recieved from the client.
   */
  class spreadsheet_controller
  {
  private:
    /**
     * This controler's pool of spreadsheets it can access.
     */
    spreadsheet_pool *sheets;

    /**
     * A map of all currently connected to clients, mapped to the spreadsheet name that they're 
     * currently editing.
     */
    std::unordered_map<std::string, client> clients;
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
    spreadsheet_controller(spreadsheet_pool *sheet);

    /**
     * Takes in a properly formatted message struct, and performs the actions required to handle it. 
     * For example, if it recieves an EDIT, the controller will add those changes to the model, and then
     * call its send_all listener to inform all the clients of the change.
     */
    void handle_message(message msg);
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

    /**
     * Registers with the controller the function it should use when it wants to send all clients a message.
     */
    void register_send_all(std::function<void(message)> func);
    /**
     * Registers with the controller the function it should use when it wants to send a specific client a message.
     */
    void register_send_client(std::function<void(std::string, message)> func);
  };
}

#endif
