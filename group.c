#include "group.h"

void initTables(sqlite3 * db, fC callback){

  system("./ttr");

  char * glist = "CREATE TABLE IF NOT EXISTS GROUPLIST (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL,"\
                    "DISPLAY  INTEGER          NOT NULL)";


  char * gtask = "CREATE TABLE IF NOT EXISTS GROUPTASK (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TIMEID   INTEGER          NOT NULL,"\
                    "FOREIGN KEY(TIMEID) REFERENCES TIME(ID));";

   sqlQuery(db, callback, glist);
   sqlQuery(db, callback, gtask);
   foreignKey(db, 1);

}
