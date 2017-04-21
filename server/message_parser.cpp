#include "message_parser.h"
#include <sstream>

using namespace CS3505;
using namespace std;


/**
 * Takes in a RAD protocol formatted message string, and passes back a message struct corresponding to the 
 * contents of the afformentioned message string.
 */
message message_parser::parse_message(string string_message)
{
  //Put our message into a stringstream
  stringstream sstream(string_message);
  //Read until the first tab character.
  string token;
  getline(sstream, token, '\t');
  //Declare message type depending on what we parsed.
  message msg;
  if (token == "Edit")
    {
      msg.type = message_type::EDIT;
      //Parse out cell name and cell contents
      getline(sstream, msg.cell_name, '\t');
      getline(sstream, msg.cell_contents, '\t');
    }
  else if (token == "Undo")
    {
      msg.type = message_type::UNDO;
      //Do nothing else.
    }
  else if (token == "Connect")
    {
      msg.type = message_type::CONNECT;
      //Parse out the spreadsheet name
      getline(sstream, msg.sheet_name, '\t');
    }
  else if (token == "IsTyping")
    {
      msg.type = message_type::ISTYPING;
      //Parse out the client ID and cell name.
      getline(sstream, msg.identifier, '\t');
      getline(sstream, msg.cell_name, '\t');
    }
  else if (token == "DoneTyping")
    {
      msg.type = message_type::DONETYPING;
      //Parse out the client ID and cell name.
      getline(sstream, msg.identifier, '\t');
      getline(sstream, msg.cell_name, '\t');
    }
  else
    {
      //Error state.
      msg.type = message_type::ERROR;
    }
  return msg;
}
