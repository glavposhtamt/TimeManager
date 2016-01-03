#include "group.h"

void initTables(sqlite3 * db, fC callback){

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

void deleteGroup(sqlite3 * db, fC callback, int id){
   /*
    * int id:
    * 0 - remove row
    * 1 - remove all
    */

   foreignKey(db, 0);

   if(id && id > 0)
        sqlQuery(db, callback, "DELETE FROM GROUPLIST WHERE ID = %d; DELETE FROM GROUPTASK WHERE TIMEID = %d;", id, id);

    else if(id == -1)
        sqlQuery(db, callback, "DELETE FROM GROUPLIST; DELETE FROM GROUPTASK;");

   foreignKey(db, 1);
}
