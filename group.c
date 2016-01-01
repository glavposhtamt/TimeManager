#include "group.h"

void initTables(sqlite3 * db, fC callback){

  char * group = "CREATE TABLE IF NOT EXISTS GROUPTASK (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TASKID   INTEGER          NOT NULL);";

   sqlQuery(db, callback, group);
   foreignKey(db, 1);

}
