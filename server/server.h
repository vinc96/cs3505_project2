//Written by Josh Christensen (u0978248)
#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "socket_manager.h"
#include "spreadsheet_controller.h"
#include "spreadsheet_pool.h"

namespace CS3505
{
  class server
  {
  private:
    //The socket manager we use to manage all of our connections.
    socket_manager* networking;
    //The controller we use to communicate between the networking and the spreadsheet pool..
    spreadsheet_controller* controller;
    //The thread pool that we store all of our spreadsheets in.
    spreadsheet_pool* spreadsheets;
    /*
     * The function that's called when a new client connects on the socket. Should handle sending
     * initialization data to the client.
     *
     * Parameters:
     * spreadsheet_name: The name of the spreadsheet the client wants to connect to.
     * client_identifier: The identifier that the socket_manager has assigned to this specific client.
     *  Used as a parameter in send_message to send data to a specific client.
     */
    void client_connected(std::string);
    /*
     * The function that's called when we recieve a complete, terminated message from some client.
     *
     * Parameters:
     * client_identifier: the client identifier of the client that disconnected.
     * message: The message recieved from the client.
     */
    void message_received(std::string client_identifier, std::string message);

    /*
     * The function that's called when a client's socket connection is lost (either through
     * disconnection, or network issues).
     *
     * Parameters:
     * client_identifier: the client identifier of the client that disconnected.
     */
    void client_disconnected(std::string);
    

  public:
    /*
     * Starts our server.
     * Returns true if our server is started, false otherwise.
     */
    bool start();

    /*
     * Tells the server to save all data to disk, disconnect clients, close the socket,
     * and shut down the server.
     * Returns true if the server shut down gracefully, false otherwise.
     */
    bool stop();

    /*
     * Returns true if the server is currently running, false otherwise.
     */
    bool is_running();
  };
}

#endif
