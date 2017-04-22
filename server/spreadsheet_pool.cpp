#include "spreadsheet_pool.h"

using namespace CS3505;
using namespace std;

spreadsheet_pool::spreadsheet_pool()
{
  int rc = sqlite3_open("db.sqlite", &db);
  if( rc ){
    //print error to logger
    sqlite3_close(db);
  }
}

spreadsheet_pool::~spreadsheet_pool()
{
  sqlite3_close(db);
}

bool spreadsheet_pool::edit_sheet(string sheet_name, string cell_name, string cell_contents)
{

}

message spreadsheet_pool::undo_last_change_on_sheet(string sheet_name)
{

}

message spreadsheet_pool::get_sheet_contents(string sheet_name)
{

}

void spreadsheet_pool::new_spreadsheet(string sheet_name)
{

}

message spreadsheet_pool::get_cell_for_sheet(string sheet_name, string cell_name)
{

}
