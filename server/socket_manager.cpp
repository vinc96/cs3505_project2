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

}
/*
 * The function called when it's time to start accepting sockets.
 */
void socket_manager::start_accept()
{
  
}

/*
 * The function called when we need to accept a new socket connection. Passed too boost's async_accept. 
 */
void socket_manager::accept_socket(const boost::system::error_code &error_code, socket_state *socket)
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
      

      mtx.unlock();
    }
}

/*
 * The function called when we need to handle data recieved on an incoming socket.
 */
void socket_manager::read_data(const boost::system::error_code &error_code, socket_state *socket)
{
  
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
  //Create an endpoint
  our_endpoint = new ip::tcp::endpoint(ip::tcp::v4(), PORT);
  //Create an acceptor to accpet connecting sockets
  our_acceptor = new ip::tcp::acceptor(our_io_service, *our_endpoint);
  //Spawn a new thread to accept sockets.
}

/*
 * Description: The destructor for the socket_manager class. Cleans up all used system resources.
 */
socket_manager::~socket_manager()
{
  //Clean up our endpoint
  delete(our_endpoint);
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
