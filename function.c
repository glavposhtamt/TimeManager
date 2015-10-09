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

void initTable(sqlite3 * db, int (* callback)(void *, int, char **, char **)){
   char * sql = "CREATE TABLE TIME("  \
                "ID       INT PRIMARY KEY  NOT NULL," \
                "DATE     datetime         NOT NULL," \
                "MESSAGE  CHAR(100)        NOT NULL);";

   sqlQuery(db, sql, callback);
    
   sqlite3_close(db);
    
}

void addTask(char ** msg, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    
   char * sql = "INSERT INTO TIME (ID,DATE,MESSAGE) "  \
                "VALUES (1, datetime('now'), 'Test');";
   sqlQuery(db, sql, callback);
    
   sqlite3_close(db);
}


void selectAll(sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    const char * sql = "SELECT * from TIME";
    
    sqlQuery(db, sql, callback);
    
    sqlite3_close(db);
}
