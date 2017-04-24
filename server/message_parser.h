#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <string>
#include <unordered_map>
namespace CS3505
{

  /*
   * Represents the type of a message.
   * Client->Server Message types are EDIT, UNDO, CONNECT.
   * Server->Client Message types are CHANGE, STARTUP.
   * Shared message types are ISTYPING and DONETYPING.
   * For errors, there's also an ERROR state.
   */
  enum message_type {EDIT, UNDO, CONNECT,
		     CHANGE, STARTUP,
		     ISTYPING, DONETYPING,
		     MESSAGE_ERROR};


  /*
   * Represents a parsed message from a client. Has an Enum representing the type of message,
   * and a number of fields that will be filled in, depending on message type.
   */
  struct message
  {
    //The type of the message.
    message_type type;
    //The client identifier. Should always be filled.
    std::string identifier;
    //The cell name. Only filled if the message is an Edit, Change, IsTyping, or DoneTyping.
    std::string cell_name;
    //The cell contents. Only filled if the message is an Edit.
    std::string cell_contents;
    //Spreadsheet name. Only filled if the message is a Connect.
    std::string sheet_name;
    //An unordered map of cell names and cell contents. Only filled if the message is a Startup
    std::unordered_map<std::string, std::string> cells;
  };
  /**
   * A class that has a single static method, parse_message, that takes a string as a parameter, and returns
   * a message struct that corresponds to the parsed state of that message. If the message has errors parsing,
   * returns ERROR.
   */
  class message_parser
  {
  private:

  public:
    /**
     * Takes in a RAD protocol formatted message string that's intended to be received by the server 
     * and the client identifier it was sent from,  and passes back a message struct corresponding to 
     * the contents of the afformentioned message string.
     */
    static message parse_client_message(std::string string_msg, std::string string_ident);

    /**
     * Takes in a RAD protocol formatted message string, and passes back a message struct corresponding to the
     * contents of the afformentioned message string.
     */
    static std::string encode_client_message(message message);
  };
}
#endif
