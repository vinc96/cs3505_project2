//Written by Josh Christensen (u0978248)
#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <iostream>
#include <fstream>


namespace CS3505
{
  enum loglevel
  { ERROR = 0, WARNING = 1, INFO = 2, ALL = 3};
  /**
   * Provides a thread safe, shared resource to allow users of the class to write to both the logfile
   * and console at the same time. This is a singleton class that, once initialized, can be 
   * the only copy in the process.
   */
  class logger
  {
  private:
    static logger* open_logger;
    static bool created;
    static std::mutex mtx;
    std::ofstream logfile;
    loglevel file_log_level;
    loglevel console_log_level;
  
    /*
     * Builds a new logger object with the specified filename, and the specified loglevels for file and console
     * 
     * Parameters:
     * filename: The name of the file to open.
     * file_loglevel: The loglevel of the items we write to file.
     * console_loglevel: The loglevel of the items we write to console.
     */
    logger(std::string filename, loglevel file_loglevel, loglevel console_loglevel);
  public:
  
    /*
     * If a logger object already exists, or if there's an issue opening the file, returns NULL. 
     * Otherwise: returns a new logger object with the specified filename, and the specified loglevels 
     * for file and console.
     * 
     * Parameters:
     * filename: The name of the file to open.
     * file_loglevel: The loglevel of the items we write to file.
     * console_loglevel: The loglevel of the items we write to console.
     */
    static logger* make_logger(std::string filename, loglevel file_loglevel, loglevel console_loglevel);
  
    /*
     * If an open logger doesn't exist, returns null. Otherwise, returns a reference to that logger.
     */
    static logger* get_logger();

    /*
     * Cleans up the logger object, freeing its internal data.
     */
    ~logger();

    /*
     * Writes the specified string to the logging resource. If the loglevel passed is higher priority than the 
     * loglevels the logger was initialized with (ERROR > WARNING > INFO > ALL), the message may be written to 
     * the console and/or the logfile.
     */
    void log(std::string message, loglevel level);
  }; 

}

#endif
