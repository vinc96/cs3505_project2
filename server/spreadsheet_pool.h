#ifndef SPREADSHEET_POOL_H
#define SPREADSHEET_POOL_H

#include <string>
#include <tuple>
#include "sqlite3.h"
#include "message_parser.h"

namespace CS3505
{
  /**
   * Represents the spreadsheets stored on some server.
   */
  class spreadsheet_pool
  {
  private:
    void new_spreadsheet(std::string sheet_name);
    sqlite3 *db;

  public:
    /**
     * Creates an empty spreadsheet pool.
     */
    spreadsheet_pool();
    ~spreadsheet_pool();
    bool edit_sheet(std::string sheet_name, std::string cell_name, std::string cell_contents);
    message undo_last_change_on_sheet(std::string sheet_name);
    message get_sheet_contents(std::string sheet_name);
    message get_cell_for_sheet(std::string sheet_name, std::string cell_name);
  };
}

#endif
