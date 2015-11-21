#include "header.h"

void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **), void * data){
    int rc;
    char * zErrMsg = 0;
    
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Operation done successfully\n");
    }
    
}

values * selectFromTableById( sqlite3 * db, const char * sql ){
    int rc, nrows, ncols;
    char ** result, * errmsg;
    values * val = malloc(sizeof(values));
    
    rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &errmsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_free_table(result);
        exit(1);
    }else{
        
        val->result = result;
        val->columns = ncols;
        val->rows = nrows;
        
        return val;        
    }    
}

void freeStructValues(values * val){
    sqlite3_free_table(val->result);
    free(val);
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

   sqlQuery(db, time, callback, NULL);    
   sqlQuery(db, task, callback, NULL);
    
}

void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[124];
    char * sql = "INSERT INTO TIME (DATE, MESSAGE) VALUES (date('now')";
    
    sprintf(query, "%s, '%s');", sql, msg);
            
    sqlQuery(db, query, callback, NULL);    

}


void selectAll(int flag, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    const char * sql = flag ? "SELECT * FROM TIME WHERE date = date('now')" : "SELECT * from TIME;";
    
    sqlQuery(db, sql, callback, NULL); 
    
}

void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int flag){    
    if(!flag && id && id > 0){
        char query[64];
        
        sprintf(query, "DELETE FROM TIME WHERE ID = %d;", id);
        sqlQuery(db, query, callback, NULL); 
        
        sprintf(query, "DELETE FROM TASK WHERE TIMEID = %d;", id);
        sqlQuery(db, query, callback, NULL); 
    }
    if(flag){
        sqlQuery(db, "DELETE FROM TIME; DELETE FROM TASK;", callback, NULL);
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
    sqlQuery(db, query, callback, NULL); 
}
    
void taskToLastday(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)) {
    char query[64];
    
    sprintf(query, "UPDATE TIME SET DATE = date('now') WHERE ID = %d;", id);
    sqlQuery(db, query, callback, NULL); 
}

void insertTimeRange(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[64];
    
    if(id < 0) id *= -1;
    //Бажочек
    if(!id) sprintf(query, "INSERT INTO TASK (TIMEID, START) VALUES (%d, datetime('now'));", id);
    else sprintf(query, "UPDATE TASK SET STOP = datetime('now') WHERE TIMEID = %d;", id);
    sqlQuery(db, query, callback, NULL); 
}

double getPeriod(char * dateStart, char * dateStop){
    struct tm tmStart, tmStop;
    time_t timeStampStart, timeStampStop;
        
    strptime(dateStart, "%Y-%m-%d %H:%M:%S", &tmStart);
    strptime(dateStop, "%Y-%m-%d %H:%M:%S", &tmStop);

    timeStampStart = mktime(&tmStart);
    timeStampStop = mktime(&tmStop);
        
    return difftime(timeStampStop, timeStampStart);
}


void getTaskTime(char * sql, int id, sqlite3 * db){
    int i, j, proxy;
    double seconds = 0.0;
    char query[64];
    
    sprintf(query, sql, id);
    
    values * val = selectFromTableById(db, query);
    
    int count = val->columns * val->rows + val->columns;
    
    for (i = val->columns, j = 1; i < count; i++) {
        if(j == START_COLUMN_NUMBER) proxy = i;
        if(j == STOP_COLUMN_NUMBER && val->result[i] != NULL) {
  
            seconds += getPeriod(val->result[proxy], val->result[i]);
        }    
        if(j == val->columns) j = 1;
        else j++;
    }
    
    printf("%.2lf\n",  seconds / 60 / 60 );
    
    freeStructValues(val);   
}
