//Written by Josh Christensen
#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <string>

namespace CS3505
{
  /*
   * Description:
   * A struct containing callbacks that are used to process incoming data.
   * Key Terms:
   * spreadsheet_name: A unique string, corresponding to a specific spreadsheet.
   * client identifier: A string that corresponds to a specific client. client_identifiers are unique among
   * currently connected clients.
   */
  struct network_callbacks
  {
    /*
     * The function that's called when a new client connects on the socket. Should handle sending
     * initialization data to the client.
     *
     * Parameters:
     * spreadsheet_name: The name of the spreadsheet the client wants to connect to.
     * client_identifier: The identifier that the socket_manager has assigned to this specific client.
     *                Used as a parameter in send_message to send data to a specific client.
     */
    void *client_connected(std::string spreadsheet_name, std::string client_identifier);
    /*
     * The function that's called when we recieve a complete, terminated message from some client.
     *
     * Parameters:
     * message: The message recieved from the client, complete with terminating character.
     */
    void *message_recieved(std::string message);
    /*
     * The function that's called when a client's socket connection is lost (either through
     * disconnection, or network issues).
     *
     * Parameters:
     * client_identifier: the client identifier of the client that disconnected.
     */
    void *client_disconnected(std::string client_identifier);
  };

  //Responsible for accepting and keeping track of sockets.
  class socket_manager
  {
  private:
    //The terminating character that we use to determine when a message is finished.
    static const char TERM_CHAR = '\n';
    //The TCP port we're operating on.
    const int PORT = 11000;
    //The struct containing the callbacks we want to execute when we recieve activity on the socket.
    network_callbacks callbacks;

    //The function called when we need to accept a new socket connection.
    void accept_socket();

  public:
    /*
     * Description:
     * Creates a new socket_manager class, logging to the specified file.
     *
     * Parameters:
     * callbacks: The network_callbacks struct we should use for our callbacks.
     * log_file: A string, corresponding to some relative filepath that we should log our output to.
     * logged output is plaintext output, logged to the end of the file.
     */
    socket_manager(network_callbacks callbacks, std::string log_file);
    /*
     * Description: The destructor for the socket_manager class. Cleans up all used system resources.
     */
    ~socket_manager();

    /*
     * Sends the specified message string to the specified client identifier. If that client identifier
     * corresponds to a currently connected client, sends the message and returns true. Otherwise,
     * returns false.
     */
    bool send_message(std::string message, std::string client_identifier);

    /*
     * Sends a message to all connected clients. Returns true if there's at least one client. false otherwise.
     */
    bool send_all(std::string message);
  };
}


#endif
