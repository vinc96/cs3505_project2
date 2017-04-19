//Written by Josh Christensen

#include "socket_manager.h"
#include "logger.h"

using namespace CS3505;
using namespace std;
using namespace boost::asio;

/*
 * Generates a socket string name unique among the existing sockets.
 */
string generate_socket_name()
{
  return "0"; //Placeholder
}

/*
 * The function called when we need to accept a new socket connection. Passed too boost's async_accept. 
 */
void socket_manager::accept_socket(socket_state *socket, const boost::system::error_code &error_code)
{
  logger *log = logger::get_logger();
  if (error_code)
    {
      log->log(string("Socket Error: ") + error_code.message(), loglevel::ERROR);
    }
  else
    {
      mtx.lock();
      //Add the socket to our map of sockets, and start accepting sockets again.
      string name = generate_socket_name();
      sockets->emplace(name, socket); 

      //Start an async read some
      socket->socket.async_read_some(buffer(socket->buffer, buff_size), 
				     boost::bind(&socket_manager::read_data, 
						 this, socket, boost::asio::placeholders::error, 
						 boost::asio::placeholders::bytes_transferred));

      //Start accepting sockets.
      socket_state* ss = new socket_state(our_io_service, our_endpoint, buff_size);
      our_acceptor->async_accept(ss->socket, 
			     boost::bind(&socket_manager::accept_socket, this, ss, boost::asio::placeholders::error));
      
      
      mtx.unlock();
    }
}

/*
 * The function called when we need to handle data recieved on an incoming socket.
 */
void socket_manager::read_data(socket_state *socket, const boost::system::error_code &error_code, int bytes_read)
{
  //Read bytes_read bytes into the socket buffer
  
}

/*
 * Description:
 * Creates a new socket_manager class. Before this class is constructed, a logging resource should be initialized.
 *
 * Parameters:
 * callbacks: The network_callbacks struct we should use for our callbacks.
 */
socket_manager::socket_manager(network_callbacks callbacks)
  : callbacks(callbacks)
{
  //Create the map for accepting sockets.
  sockets = new SOCKETMAP();
  //Set the port for the endpoint
  our_endpoint.port(PORT);
  //Create an acceptor to accpet connecting sockets
  our_acceptor = new ip::tcp::acceptor(our_io_service, our_endpoint);
  //Start accepting sockets.
  socket_state* ss = new socket_state(our_io_service, our_endpoint, buff_size);
  our_acceptor->async_accept(ss->socket, 
			     boost::bind(&socket_manager::accept_socket, this, ss, boost::asio::placeholders::error));
}

/*
 * Description: The destructor for the socket_manager class. Cleans up all used system resources.
 */
socket_manager::~socket_manager()
{
  //Clean up our endpoint
  //Clean up all our sockets.
  for(SOCKETMAP::iterator iterator = sockets->begin(); iterator != sockets->end(); iterator++)
    {
      delete(iterator->second);
    }
  //Delete our socket map.
  delete(sockets);
}

/*
 * Sends the specified message string to the specified client identifier. If that client identifier
 * corresponds to a currently connected client, sends the message and returns true. Otherwise,
 * returns false.
 */
bool socket_manager::send_message(string message, string client_identifier)
{

}

/*
 * Sends a message to all connected clients. Returns true if there's at least one client. false otherwise.
 */
bool socket_manager::send_all(string message)
{

}
