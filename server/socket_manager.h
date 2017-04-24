//Written by Josh Christensen
#ifndef SOCKET_MANAGER_H
#define SOCKET_MANAGER_H

#include <string>
#include <sstream>
#include <unordered_map>
#include <mutex>
#include <boost/asio.hpp>
#include <thread>
#include <queue>
#include <list>

namespace CS3505
{

  /*
   * Description:
   * A struct that contains a stringstream, and a boost socket. 
   */
  struct socket_state
  {
  socket_state(boost::asio::io_service &service, boost::asio::ip::tcp::endpoint endpoint, int buf_size)
  : socket(service)//socket(service, endpoint)

    {
      //Allocate our buffer
      receive_buffer = new char[buf_size];
      }

    ~socket_state()
    {
      //Close the socket.
      socket.close();
      //Free our buffer.
      delete(receive_buffer);
      //If our send_buffers queue isn't empty, free the buffers.
      //TODO: Warning?
      while (send_buffers.size() != 0)
	{
	  delete(send_buffers.front());
	  send_buffers.pop();
	}
    }
    //The mutex that we use to avoid race conditions on buffer reads.
    std::mutex mtx;
    
    /*
     * This buffer's unique string identifier.
     */
    std::string identifier;

    /*
     *The socket that we recieve data from. 
     */
    boost::asio::ip::tcp::socket socket;
    /*
     * As we receive data from the socket, it's shuffled into this buffer, so we can handle it as
     * we recieve complete messages.
     */
    std::stringstream stream;
    /*
     * The buffer that our socket writes data in.
     */
    char* receive_buffer;
    /*
     * The buffer that our socket writes data from.
     */
    std::queue<char*, std::list<char*> > send_buffers;
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
     * Parameters (in order):
     * spreadsheet_name: The name of the spreadsheet the client wants to connect to.
     * client_identifier: The identifier that the socket_manager has assigned to this specific client.
     *  Used as a parameter in send_message to send data to a specific client.
     */
    std::function<void(std::string)> client_connected;
    /*
     * The function that's called when we recieve a complete, terminated message from some client.
     *
     * Parameters (in order):
     * client_identifier: the client identifier of the client that disconnected.
     * message: The message recieved from the client.
     */
    std::function<void(std::string, std::string)> message_received;
    /*
     * The function that's called when a client's socket connection is lost (either through
     * disconnection, or network issues).
     *
     * Parameters:
     * client_identifier: the client identifier of the client that disconnected.
     */
    std::function<void(std::string)>  client_disconnected;
  };

  //Responsible for accepting and keeping track of sockets.
  class socket_manager
  {
  private:
    //The terminating character that we use to determine when a message is finished.
    static const char TERM_CHAR = '\n';
    //The TCP port we're operating on.
    static const unsigned short PORT = 2112;
    //The size of the buffers that we're using for sockets.
    static const int buff_size = 1024;
    //The number of sockets that have connected to this manager, since the last time we had 0 clients.
    int num_connected_sockets = 0;
    //The struct containing the callbacks we want to execute when we recieve activity on the socket.
    network_callbacks callbacks;
    //The map containing our sockets, mapped to their unique identifiers.
    SOCKETMAP *sockets;
    //The thread that we use to perform the asynchronous boost io.
    std::thread *work_thread;
    //The ioservice for the networking class
    boost::asio::io_service our_io_service;
    //The endpoint we use to accept connections on port PORT.
    boost::asio::ip::tcp::endpoint our_endpoint;
    //The acceptor we use to accept connections on port PORT.
    boost::asio::ip::tcp::acceptor *our_acceptor;
    //The mutex we use to avoid race conditions
    std::mutex mtx;
    //The function we use to do background async io work. Started in its own worker thread.
    void do_work();
    //Generates a socket string name unique among existing sockets.
    std::string generate_socket_name();
    //The function called when we need to accept a new socket connection.
    void accept_socket(socket_state *socket_state, const boost::system::error_code &error_code);
    //The function called when we need to handle data recieved on an incoming socket.
    void read_data(socket_state *socket_state, const boost::system::error_code &error_code, int bytes_read);
    //The function called when we finish writing data to a socket
    void data_written(socket_state *socket_state, const boost::system::error_code &error_code, int bytes_written);
    //Handles disconnecting sockets
    void handle_disconnect(socket_state *socket_state);
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
