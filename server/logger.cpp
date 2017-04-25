//Written by Josh Christensen (u0978248)
#include "logger.h"

using namespace CS3505;

//Initialize the private static variables to be NULL, FALSE, and initialize the mtx.
logger* logger::open_logger = NULL;
bool logger::created = false;
std::mutex logger::mtx;

using namespace std;

/*
 * Builds a new logger object with the specified filename, and the specified loglevels for file and console.
 * 
 * Parameters:
 * filename: The name of the file to open.
 * file_loglevel: The loglevel of the items we write to file.
 * console_loglevel: The loglevel of the items we write to console.
 */
logger::logger(std::string filename, loglevel file_loglevel, loglevel console_loglevel)
{
  //Open the file.
  logfile.open(filename);
  //Initialize the instance variables
  file_log_level = file_loglevel;
  console_log_level = console_loglevel;
}
  
/*
 * Cleans up the logger object, freeing its internal data.
 */
logger::~logger()
{
  //Free the file. 
  logfile.close();
}


/*
 * If a logger object already exists, or if there's an issue opening the file, returns NULL. 
 * Otherwise: returns a reference to a new logger object with the specified filename, and 
 * the specified loglevels for file and console.
 *
 * WARNING: THIS METHOD IS NOT THREAD SAFE.
 *
 * Parameters:
 * filename: The name of the file to open.
 * file_loglevel: The loglevel of the items we write to file.
 * console_loglevel: The loglevel of the items we write to console.
 */
logger* logger::make_logger(std::string filename, loglevel file_loglevel, loglevel console_loglevel)
{
  if (open_logger == NULL)
    {
      open_logger = new logger(filename, file_loglevel, console_loglevel);
      return open_logger;
    }
  else
    {
      return NULL;
    }
}

/*
 * If an open logger doesn't exist, returns null. Otherwise, returns a reference to that logger.
 */
logger* logger::get_logger()
{
  return open_logger;
}

/*
 * Writes the specified string to the logging resource. If the loglevel passed is higher priority than the 
 * loglevels the logger was initialized with (ERROR > WARNING > INFO), the message may be written to the 
 * console and/or the logfile.
 *
 * Parameters:
 * message: The message we want to write to the resources.
 * level: The priority of the message.
 */
void logger::log(string message, loglevel level)
{
  string logstring = "";
 switch (level)
   {
   case ERROR: logstring = string("[ERROR]: ") + message;
     break;
   case WARNING: logstring = string("[WARNING]: ") + message;
     break;
   case INFO: logstring = string("[INFO]: ") + message;
     break;
   case ALL: logstring = string("[ALL]: ") + message;
     break;
   }
  //Write to file, if we fit the requirements
  if (level <= file_log_level)
    {
      //Lock the logging resources.
      mtx.lock();
      logfile << logstring << endl;
      mtx.unlock();
    }
  //Write to console, if we fit the requirements.
  if (level <= console_log_level)
    {
      //Lock the logging resources.
      mtx.lock();
      cout << logstring << endl;
      mtx.unlock();
    }
  
}
