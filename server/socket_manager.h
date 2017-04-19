//Written by Josh Christensen
#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <string>
#include <sstream>
#include <unordered_map>
/*
  #include <boost/asio/io_service.hpp>
  #include <boost/asio/write.hpp>
  #include <boost/asio/buffer.hpp>
  #include <boost/asio/ip/tcp.hpp>
*/
#include <boost/asio.hpp>
#include "logger.h"

namespace CS3505
{

  /*
   * Description:
   * A struct that contains a stringstream, and a boost socket. 
   */
  struct socket_state
  {
    socket_state(boost::asio::io_service service, boost::asio::ip::tcp::endpoint endpoint)
      : socket(service, endpoint)

    {
    }

    ~socket_state()
    {
      socket.close();
    }
    boost::asio::ip::tcp::socket socket;
    std::stringstream stream;
  };

  //Typedefs:
  typedef std::unordered_map<std::string, socket_state*> SOCKETMAP;

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
    static const int PORT = 2112;
    //The struct containing the callbacks we want to execute when we recieve activity on the socket.
    network_callbacks callbacks;
    //The map containing our sockets, mapped to their unique identifiers.
    SOCKETMAP *sockets;
    //The mutex we use to avoid race conditions
    std::mutex mtx;
    //The function we call when we want to start accepting socket connections.
    void start_accept();
    //The function called when we need to accept a new socket connection.
    void accept_socket(const boost::system::error_code &error_code, socket_state *socket_state);

  public:
    /*
     * Description:
     * Creates a new socket_manager class, logging to the specified logger.
     *
     * Parameters:
     * callbacks: The network_callbacks struct we should use for our callbacks.
     */
    socket_manager(network_callbacks callbacks);
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
