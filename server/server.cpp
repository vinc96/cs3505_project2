//Written by Josh Christensen

#include "server.h"
#include "logger.h"

using namespace CS3505;

bool server::start()
{
  //Create a new logger:
  logger *log = logger::make_logger("log", loglevel::ALL, loglevel::ALL);
  if (log == NULL)
    {
      std::cout << "[ERROR]: LOGGER FAILED TO START." << std::endl;
      return false;
    }
  //Log that the server started
  log->log("Server Started", loglevel::INFO);
  
  //Create the socket_manager
  
  //Insert other initialization code here.
  
  return true; 
}

bool server::stop()
{
  //Clean things up.
  
  return true; //Placeholder
}

bool server::is_running()
{
  return true; //Placeholder
}
