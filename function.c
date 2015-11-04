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
                    "DATE     datetime         NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL," \
                    "STATUS   INT              DEFAULT 0 );";
    
    char * task = "CREATE TABLE IF NOT EXISTS TASK ("  \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TIMEID   INTEGER          NOT NULL," \
                    "START    datetime         NOT NULL," \
                    "STOP     datetime         NOT NULL);";

   sqlQuery(db, time, callback);    
   sqlQuery(db, task, callback);
    
   //sqlite3_close(db);
    
}

void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    
    char * sql = "INSERT INTO TIME (DATE, MESSAGE) VALUES (datetime('now'), '";
    
    char * tmp = (char *)malloc(124);
    strcpy(tmp, sql);
    strcat(tmp, msg);
    strcat(tmp, "');");
        
    sqlQuery(db, sql, callback);    
    //sqlite3_close(db);
    
    printf("%s\n", tmp);
    free(tmp);
}


void selectAll(sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    const char * sql = "SELECT * from TIME";
    
    sqlQuery(db, sql, callback);
    
    sqlite3_close(db);
}
