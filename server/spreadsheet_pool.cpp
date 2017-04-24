#include "spreadsheet_pool.h"

using namespace CS3505;
using namespace std;


static int __last_change(void* pNames, int columns, char** data, char** columnNames)
{
    std::vector<std::string>* vNames = static_cast< std::vector<std::string>* >(pNames);
    vNames->push_back(data[1]);
    // Success:
    return 0;
}

message spreadsheet_pool::undo_last_change_on_sheet(string sheet_name)
{

}

static int __sheet_contents(void* pNames, int columns, char** data, char** columnNames)
{
    std::vector<std::string>* vNames = static_cast< std::vector<std::string>* >(pNames);
    vNames->push_back(data[1]);
    // Success:
    return 0;
}

bool spreadsheet_pool::doesSheetExist(string sheet_name)
{
    sqlite3_stmt *statement = NULL;
    const char* sql = string("SELECT count(id) FROM spreadsheets WHERE name= '" + sheet_name + "'").c_str();
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
    return count > 0;
}

message spreadsheet_pool::get_sheet_contents(string sheet_name)
{

}

static int __cell_on_sheet(void* pNames, int columns, char** data, char** columnNames)
{
    std::vector<std::string>* vNames = static_cast< std::vector<std::string>* >(pNames);
    vNames->push_back(data[1]);
    // Success:
    return 0;
}

message spreadsheet_pool::get_cell_on_sheet(string sheet_name, string cell_name)
{

}

void spreadsheet_pool::new_spreadsheet(string sheet_name)
{
  string sql = "CREATE TABLE COMPANY("  \
         "ID INT PRIMARY KEY     NOT NULL," \
         "NAME           TEXT    NOT NULL," \
         "AGE            INT     NOT NULL," \
         "ADDRESS        CHAR(50)," \
         "SALARY         REAL );";
}

bool spreadsheet_pool::add_edit(string sheet_name, string cell_name, string cell_contents)
{

}

static int __check_initialization(void* not_used, int columns, char** data, char** columnNames)
{
  return 1;
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
                                         "id INT primary_key NOT NULL," \
                                         "name TEXT)";
    char *error_message = 0;
    rc = sqlite3_exec(db, spreadsheetTableCreate, __check_initialization, 0, &error_message);
    if( rc != SQLITE_OK ){
        log->log(string("SQL Error:"+string(error_message)), loglevel::ERROR);
        sqlite3_free(error_message);
     }

     const char *editTableCreate = "CREATE TABLE IF NOT EXISTS edits(" \
                                   "id INT primary_key NOT NULL," \
                                   "spreadsheet_id INT foreign_key NOT NULL," \
                                   "cellName TEXT," \
                                   "cellContents TEXT," \
                                   "undone INTEGER)";

     error_message = 0;
     rc = sqlite3_exec(db, editTableCreate, __check_initialization, 0, &error_message);
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
