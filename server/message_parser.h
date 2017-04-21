#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

#include <string>
namespace CS3505
{

  /*
   * Represents the type of a message. Message types are EDIT, UNDO, CONNECT, ISTYPING, DONETYPING, and ERROR.
   */
  enum message_type {EDIT, UNDO, CONNECT, ISTYPING, DONETYPING, ERROR};


  /*
   * Represents a parsed message from a client. Has an Enum representing the type of message, 
   * and a number of fields that will be filled in, depending on message type.
   */
  struct message
  {
    //The type of the message.
    message_type type;
    //The client identifier. Filled if we recieve an IsTyping and a DoneTyping.
    std::string identifier;
    //The cell name. Only filled if the message is an Edit, IsTyping, or DoneTyping.
    std::string cell_name;
    //The cell contents. Only filled if the message is an Edit.
    std::string cell_contents;
    //Spreadsheet name. Only filled if the message is a Connect.
    std::string sheet_name;
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
     * Takes in a RAD protocol formatted message string, and passes back a message struct corresponding to the 
     * contents of the afformentioned message string.
     */
    static message parse_message(std::string string_msg);
  };
}
#endif
