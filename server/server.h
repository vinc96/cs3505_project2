//Written by Josh Christensen
#ifndef SERVER_H
#define SERVER_H

namespace CS3505
{
  class server
  {
  private:


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
