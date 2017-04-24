#include "spreadsheet_pool.h"

using namespace CS3505;
using namespace std;


static int __generic_callback(void* not_used, int columns, char** data, char** columnNames)
{
  return 1;
}

static int __undo_cell_name(void* cell_message, int columns, char** data, char** columnNames)
{
    message* new_cell = static_cast< message* >(cell_message);
    new_cell->cell_name = data[0];
    // Success:
    return 0;
}

static int __last_value(void* cell_message, int columns, char** data, char** columnNames)
{
    message* new_cell = static_cast< message* >(cell_message);
    new_cell->cell_contents = data[0];
    // Success:
    return 0;
}

message spreadsheet_pool::undo_last_change_on_sheet(string sheet_name)
{
  log->log("Undoing last change for: " + sheet_name, loglevel::ALL);
  if(!doesSheetExist(sheet_name))
  {
    log->log(string("Client attempted undo on non-existant spreasheet :"+sheet_name), loglevel::ERROR);
    message error_message;
    error_message.type = message_type::MESSAGE_ERROR;
    return error_message;
  }
  message new_contents;
  new_contents.type = message_type::MESSAGE_ERROR;
  new_contents.cell_name = "";
  /*const char *undo_and_query = string("UPDATE edits SET undone = 1 WHERE id = "\
                                        "(SELECT max(id) FROM edits "\
                                          "WHERE undone IS NULL AND spreadsheet_id = "\
                                              "(SELECT id FROM spreadsheets WHERE name = '"+sheet_name+"')"\
                                        "); "\
                                      "SELECT cell_name, cell_contents FROM table ORDER BY id DESC LIMIT 1 WHERE cell_name = undone is NULL").c_str();
  */
  const char * get_undo_cell_name = sqlite3_mprintf(string("SELECT cell_name FROM edits "\
                                           "WHERE undone IS NULL AND spreadsheet_id = "\
                                           "(SELECT id FROM spreadsheets WHERE name = %Q) "\
                                           "ORDER BY id DESC LIMIT 1").c_str(), sheet_name.c_str());
  char *error_message = 0;
  int rc = sqlite3_exec(db, get_undo_cell_name, __undo_cell_name, &new_contents, &error_message);
  if( rc != SQLITE_OK ){
      log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
      sqlite3_free(error_message);
      message e_message;
      e_message.type = message_type::MESSAGE_ERROR;
      return e_message;
  }
  if(new_contents.cell_name == "")
  {
    return new_contents;
  }
  else
  {
    const char * set_undone_and_query = sqlite3_mprintf(string("UPDATE edits SET undone = 1 WHERE id = "\
                                               "(SELECT max(id) FROM edits "\
                                                 "WHERE undone IS NULL AND spreadsheet_id = "\
                                                     "(SELECT id FROM spreadsheets WHERE name = %Q)"\
                                               "); "\
                                               "SELECT cell_contents FROM edits "\
                                               "WHERE cell_name = %Q AND "\
                                               "undone is NULL AND spreadsheet_id = "\
                                               "(SELECT id FROM spreadsheets WHERE name = '"+sheet_name+"')").c_str(), sheet_name.c_str(), new_contents.cell_name.c_str());
    char *error_message = 0;
    int rc = sqlite3_exec(db, set_undone_and_query, __last_value, &new_contents, &error_message);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
        sqlite3_free(error_message);
        message e_message;
        e_message.type = message_type::MESSAGE_ERROR;
        return e_message;
    }
  }
  return new_contents;
}

static int __sheet_contents(void* cells, int columns, char** data, char** columnNames)
{
    std::unordered_map<std::string, std::string>* contents = static_cast< std::unordered_map<std::string, std::string>* >(cells);
    contents->insert(std::make_pair<std::string,std::string>(string(data[0]), string(data[1])));
    // Success:
    return 0;
}

bool spreadsheet_pool::doesSheetExist(string sheet_name)
{
    sqlite3_stmt *statement = NULL;
    const char* sql = sqlite3_mprintf(string("SELECT count(id) FROM spreadsheets WHERE name= %Q").c_str(), sheet_name.c_str());
    int rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:" + string(sqlite3_errmsg(db))), loglevel::ERROR);
    }
    rc = sqlite3_step(statement);
    int count = 0;
    if(rc == SQLITE_ROW)
    {
      count = sqlite3_column_int(statement, 0);
    }
    sqlite3_finalize(statement);
    return count > 0;
}

message spreadsheet_pool::get_sheet_contents(string sheet_name)
{
    if(!doesSheetExist(sheet_name))
    {
      new_spreadsheet(sheet_name);
    }
    log->log(string("Getting sheet contents for: "+sheet_name), loglevel::ALL);
    message spreadsheet_contents;
    spreadsheet_contents.type = message_type::STARTUP;
    std::unordered_map<std::string, std::string> cells;
    const char *get_sheet_contents = sqlite3_mprintf(string("SELECT DISTINCT cell_name, cell_contents FROM edits " \
                                         "WHERE undone is NULL AND " \
                                         "spreadsheet_id = (SELECT id FROM spreadsheets WHERE name = %Q) "\
                                         "ORDER BY id DESC").c_str(), sheet_name.c_str());
    char *error_message = 0;
    int rc = sqlite3_exec(db, get_sheet_contents, __sheet_contents, &cells, &error_message);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
        sqlite3_free(error_message);
        message e_message;
        e_message.type = message_type::MESSAGE_ERROR;
        return e_message;
    }
    spreadsheet_contents.cells = cells;
    return spreadsheet_contents;
}

static int __cell_on_sheet(void* contents, int columns, char** data, char** columnNames)
{
    std::string* cell_contents = static_cast< std::string* >(contents);
    *cell_contents = string(data[0]);
    // Success:
    return 0;
}

message spreadsheet_pool::get_cell_on_sheet(string sheet_name, string cell_name)
{
  if(!doesSheetExist(sheet_name))
  {
    log->log(string("Client attempted edit on non-existant spreasheet :"+sheet_name), loglevel::ERROR);
    message error_message;
    error_message.type = message_type::MESSAGE_ERROR;
    return error_message;
  }
  log->log(string("Getting sheet contents for: "+sheet_name), loglevel::ALL);
  string cell_contents = "";
  const char *get_cell = sqlite3_mprintf(string("SELECT cell_contents FROM edits "
                                "WHERE undone is NULL AND e.cell_name = %Q AND "\
                                "spreadsheet_id = (SELECT id from spreadsheets WHERE name = %Q) " \
                                "ORDER BY max(e.id)").c_str(), cell_name.c_str(), sheet_name.c_str());
  char *error_message = 0;
  int rc = sqlite3_exec(db, get_cell, __cell_on_sheet, &cell_contents, &error_message);
  if( rc != SQLITE_OK ){
      log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
      sqlite3_free(error_message);
      message e_message;
      e_message.type = message_type::MESSAGE_ERROR;
      return e_message;
  }
  message single_cell;
  single_cell.type = message_type::CHANGE;
  single_cell.cell_name = cell_name;
  single_cell.cell_contents = cell_contents;
  return single_cell;
}

void spreadsheet_pool::new_spreadsheet(string sheet_name)
{
  log->log("Creating new spreadsheet named: " + sheet_name, loglevel::INFO);

  const char *spreadsheetTableCreate = sqlite3_mprintf(string("INSERT INTO spreadsheets (name) VALUES (%Q)").c_str(), sheet_name.c_str());
  char *error_message = 0;
  int rc = sqlite3_exec(db, spreadsheetTableCreate, __generic_callback, 0, &error_message);
  if( rc != SQLITE_OK ){
      log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
      sqlite3_free(error_message);
   }
}

static int __get_id(void* log_ptr, int columns, char** data, char** columnNames)
{
    logger* log = static_cast< logger* >(log_ptr);
    log->log("Spreadsheet_id value: " + string(data[0]), loglevel::ALL);
    // Success:
    return 0;
}

message spreadsheet_pool::add_edit(string sheet_name, string cell_name, string cell_contents)
{
  log->log("Adding edit on sheet: " + sheet_name + " for cell: " + cell_name + " with contents: " + cell_contents, loglevel::ALL);

  /*const char *spreadsheetTableCreate = sqlite3_mprintf(string("INSERT INTO edits " \
                                              "(cell_name,cell_contents,spreadsheet_id) VALUES"\
                                              "(%Q,%Q,"\
                                              "(SELECT id "\
                                              "FROM spreadsheets " \
                                              "WHERE name = %Q))").c_str(), cell_name.c_str(), cell_contents.c_str(), sheet_name.c_str());
  */
  char *error_message = 0;
  const char *spreadsheetTableCreate = sqlite3_mprintf(string("SELECT id "\
                                              "FROM spreadsheets " \
                                              "WHERE name = %Q").c_str(), sheet_name.c_str());
  int rc = sqlite3_exec(db, spreadsheetTableCreate, __get_id, log, &error_message);
  if( rc != SQLITE_OK ){
      log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
      sqlite3_free(error_message);
      message e_message;
      e_message.type = message_type::MESSAGE_ERROR;
      return e_message;
   }
   message change_message;
   change_message.type = message_type::CHANGE;
   change_message.cell_name = cell_name;
   change_message.cell_contents = cell_contents;
   return change_message;
}

bool spreadsheet_pool::doesTableExist(string table_name)
{
    sqlite3_stmt *statement = NULL;
    const char* sql = string("SELECT count(*) FROM sqlite_master WHERE type='table' AND name= '" + table_name + "'").c_str();
    int rc = sqlite3_prepare_v2(db, sql, -1, &statement, 0);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:" + string(sqlite3_errmsg(db))), loglevel::ERROR);
    }
    rc = sqlite3_step(statement);
    int count = 0;
    if(rc == SQLITE_ROW)
    {
      count = sqlite3_column_int(statement, 0);
    }
    sqlite3_finalize(statement);
    return count > 0;
}

spreadsheet_pool::spreadsheet_pool()
{
  log = logger::get_logger();
  int rc = sqlite3_open("db.sqlite", &db);
  if( rc ){
    log->log(string("Database failled to open."), loglevel::ERROR);
    sqlite3_close(db);
  }

  if(!doesTableExist("spreadsheets") || !doesTableExist("edits"))
  {
    log->log("Initializing Database Tables", loglevel::INFO);

    const char *spreadsheetTableCreate = "CREATE TABLE IF NOT EXISTS spreadsheets(" \
                                         "id INT primary_key," \
                                         "name TEXT)";
    char *error_message = 0;
    rc = sqlite3_exec(db, spreadsheetTableCreate, __generic_callback, 0, &error_message);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
        sqlite3_free(error_message);
     }

     const char *editTableCreate = "CREATE TABLE IF NOT EXISTS edits(" \
                                   "id INT primary_key," \
                                   "spreadsheet_id INT foreign_key NOT NULL," \
                                   "cell_name TEXT," \
                                   "cell_contents TEXT," \
                                   "undone INTEGER)";

     error_message = 0;
     rc = sqlite3_exec(db, editTableCreate, __generic_callback, 0, &error_message);
     if( rc != SQLITE_OK ){
       log->log(string("SQL Error:" + string(error_message)), loglevel::ERROR);
       sqlite3_free(error_message);
    }
  }

}

spreadsheet_pool::~spreadsheet_pool()
{
  log->log(string("Database closed."), loglevel::ALL);
  sqlite3_close(db);
}
