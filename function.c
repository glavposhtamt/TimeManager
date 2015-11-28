#include "header.h"

void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **), void * data){
    int rc;
    char * zErrMsg = 0;
    
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

values * selectFromTable( sqlite3 * db, const char * sql ){
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


void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **)){
   char * time = "CREATE TABLE IF NOT EXISTS TIME (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "DATE     date             NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL," \
                    "STATUS   INTEGER          DEFAULT 0);";
    
  char * task = "CREATE TABLE IF NOT EXISTS TASK ("  \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TIMEID   INTEGER          NOT NULL," \
                    "START    datetime         NOT NULL," \
                    "STOP     datetime           );";

  char * targ = "CREATE TABLE IF NOT EXISTS TARGET (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "DATE     date             NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL);";

   sqlQuery(db, time, callback, NULL);    
   sqlQuery(db, task, callback, NULL);
   sqlQuery(db, targ, callback, NULL);
}

void addDoing(char * msg, char * table, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[256];
    
    sprintf(query, "INSERT INTO %s (DATE, MESSAGE) VALUES (date('now'), '%s')", table, msg);
            
    sqlQuery(db, query, callback, NULL);    
}

void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int flag){    
   /*
    * int flag:
    * 0 - remove row
    * 1 - remove all
    */ 
    
    if(!flag && id > 0){
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

void taskToLastday(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)) {
    char query[64];
    
    sprintf(query, "UPDATE TIME SET DATE = date('now') WHERE ID = %d;", id);
    sqlQuery(db, query, callback, NULL); 
}

void updateStatus(int id, int status, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    
    /*
     * int status:
     * status == 1 - active
     * status == 0 - no active
     */

    char query[64];
    
    sprintf(query, "UPDATE TIME SET STATUS = %d WHERE ID = %d;", status,  id);
    sqlQuery(db, query, callback, NULL); 
}

void startStop(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **)){
    char query[128];
    
    sprintf(query, "select count(*) as count from TASK where TIMEID = %d AND STOP IS NULL;", id);
    
    values * val = selectFromTable(db, query);
    int flag = atoi(val->result[1]);

    updateStatus(id, !flag, db, callback);

    /*
     * int flag:
     * flag == FALSE - INSERT
     * flag == TRUE  - UPDATE
     */ 
    
    if(!flag) sprintf(query, "INSERT INTO TASK (TIMEID, START) VALUES (%d, datetime('now'));", id);
    if(flag) sprintf(query, "UPDATE TASK SET STOP = datetime('now') WHERE TIMEID = %d AND STOP IS NULL;", id);

    sqlQuery(db, query, callback, NULL);

    freeStructValues(val);
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


double getTaskTime(char * sql, int id, sqlite3 * db){
    int i, j, proxy;
    double seconds = 0.0;
    char query[64];
    
    sprintf(query, sql, id);
    
    values * val = selectFromTable(db, query);
    
    int count = val->columns * val->rows + val->columns;
    
    for (i = val->columns, j = 1; i < count; i++) {
        if(j == 1) proxy = i;
        if(j == 2 && val->result[i] != NULL) {
  
            seconds += getPeriod(val->result[proxy], val->result[i]);
        }    
        if(j == val->columns) j = 1;
        else j++;
    }
    
    freeStructValues(val); 
   
    return seconds;
}

cl secToTime(double seconds){
    long s = (long)seconds;
    int allMin;
    cl hms;

    hms.sec = s >= 60 ? s % 60 : s;
    allMin = s / 60;
    hms.min = allMin >= 60 ? allMin % 60 : allMin;
    hms.hours = allMin / 60;    

    return hms;
}

void printTable(char * sql, int id, sqlite3 * db){
    int i, j, timeId;
    double seconds = 0.0;
    values * val;
    char query[128];

    if(id > 0) {
        sprintf(query, sql, id);
        val = selectFromTable(db, query);
    } else val = selectFromTable(db, sql);

    int count = val->columns * val->rows + val->columns;
    
    for (i = val->columns, j = 1; i < count; i++) {
        if(j == 1) {
          timeId = atoi(val->result[i]);
          printf("[%d]\t", timeId);
          j++;
          continue;
        } 

        if(j == val->columns) {
            printf("%s\n", val->result[i]);
            j = 1;
        } else {
            seconds = getTaskTime("select START, STOP from TASK where TIMEID = %d;", timeId, db);
            cl hms = secToTime(seconds);
            printf("%.2d:%.2d:%.2d\t", hms.hours, hms.min, hms.sec);
            printf("%s\t", atoi(val->result[i]) ? "Start" : "Pause");
            j++;
        }
    }

    freeStructValues(val);   
}

void deleteTarget(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int flag){    
   /*
    * int flag:
    * 0 - remove row
    * 1 - remove all
    */ 
    
    if(!flag && id > 0){
        char query[64];
        
        sprintf(query, "DELETE FROM TARGET WHERE ID = %d;", id);
        sqlQuery(db, query, callback, NULL); 
    }
    if(flag){
        sqlQuery(db, "DELETE FROM TARGET;", callback, NULL);
    }
}

