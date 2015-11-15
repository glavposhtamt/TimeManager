#include "header.h"


void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **)){
    int rc;
    char * zErrMsg = 0;
    const char * data = "Callback function called";
    
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    
}

/*Продумать открытие/закрытие соединения*/

void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **)){
   char * time = "CREATE TABLE IF NOT EXISTS TIME (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "DATE     date             NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL," \
                    "STATUS   INT              DEFAULT 0 );";
    
  char * task = "CREATE TABLE IF NOT EXISTS TASK ("  \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TIMEID   INTEGER          NOT NULL," \
                    "START    datetime         NOT NULL," \
                    "STOP     datetime           );";

   sqlQuery(db, time, callback);    
   sqlQuery(db, task, callback);
    
   
    
}

void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[124];
    char * sql = "INSERT INTO TIME (DATE, MESSAGE) VALUES (date('now')";
    
    sprintf(query, "%s, '%s');", sql, msg);
            
    sqlQuery(db, query, callback);    

}


void selectAll(int flag, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    const char * sql = flag ? "SELECT * FROM TIME WHERE date = date('now')" : "SELECT * from TIME;";
    
    sqlQuery(db, sql, callback);
    
}

void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int flag){    
    if(!flag && id && id > 0){
        char query[64];
        
        sprintf(query, "DELETE FROM TIME WHERE ID = %d;", id);
        sqlQuery(db, query, callback);
        
        sprintf(query, "DELETE FROM TASK WHERE TIMEID = %d;", id);
        sqlQuery(db, query, callback);
    }
    if(flag){
        sqlQuery(db, "DELETE FROM TIME; DELETE FROM TASK;", callback);
    }
}

void updateStatusTask(int id, int status, sqlite3 * db, int (* callback)(void *, int, char **, char **)) {
    
    /* 
    *  int status, values:
    *  0 - No active
    *  1 - Active
    */
    
    status = status > 0 ? 1 : 0;
    
    char query[64];
    sprintf(query, "UPDATE TIME SET STATUS = %d WHERE ID = %d;", status, id);
    sqlQuery(db, query, callback);
}
    
void taskToLastday(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)) {
    char query[64];
    
    sprintf(query, "UPDATE TIME SET DATE = date('now') WHERE ID = %d;", id);
    sqlQuery(db, query, callback);
}

void insertTimeRange(int startEnd, int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[64];
    
    if(!startEnd) sprintf(query, "INSERT INTO TASK (TIMEID, START) VALUES (%d, datetime('now'));", id);
    else sprintf(query, "UPDATE TASK SET STOP = datetime('now') WHERE TIMEID = %d;", id);
    sqlQuery(db, query, callback);
}

void getTaskTime(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[64];
    
    sprintf(query, "SELECT START, STOP FROM TASK WHERE TIMEID = %d;", id);
    sqlQuery(db, query, callback);
}