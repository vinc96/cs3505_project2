#include "message_parser.h"
#include <sstream>

using namespace CS3505;
using namespace std;


/**
 * Takes in a RAD protocol formatted message string that's intended to be received by the server 
 * and the client identifier it was sent from,  and passes back a message struct corresponding to 
 * the contents of the afformentioned message string.
 */
message message_parser::parse_client_message(string string_message, string ident)
{
  //Put our message into a stringstream
  stringstream sstream(string_message);
  //Read until the first tab character.
  string token;
  getline(sstream, token, '\t');
  //Declare message type depending on what we parsed.
  message msg;
  msg.identifier = ident;
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
      msg.type = message_type::MESSAGE_ERROR;
    }
  return msg;
}

/**
 * Takes in a message struct that's intended to be sent to a client, 
 * and returns a RAD protocl formatted message string that can be sent out to clients.
 */
string message_parser::encode_client_message(message message)
{
  stringstream sstream;
  //Build a message.
  switch (message.type)
    {
    case message_type::CHANGE:
      sstream << "Change\t";
      sstream << message.cell_name << "\t";
      sstream << message.cell_contents << "\t";
      sstream << "\n";
      break;
    case message_type::STARTUP:
      sstream << "Startup\t";
      sstream << message.identifier << "\t";
      //Add all the cells to the message.
      for(unordered_map<string, string>::iterator iterator = message.cells.begin(); 
	  iterator != message.cells.end(); iterator++)
	{
	  //Write the cell name.
	  sstream << iterator->first << "\t";
	  //Write the cell contents.
	  sstream << iterator->second << "\t";
	}
      sstream << "\n";
      break;
    case message_type::ISTYPING:
      sstream << "IsTyping\t";
      sstream << message.identifier << "\t";
      sstream << message.cell_name << "\t";
      sstream << "\n";
      break;
    case message_type::DONETYPING:
      sstream << "DoneTyping\t";
      sstream << message.identifier << "\t";
      sstream << message.cell_name << "\t";
      sstream << "\n";
      break;
    }
  return sstream.str();
}
