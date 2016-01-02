#include "header.h"

void initTables(sqlite3 * db, fC callback){
   char * time = "CREATE TABLE IF NOT EXISTS TIME (" \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "DISPLAY  INTEGER          NOT NULL," \
                    "MESSAGE  CHAR(100)        NOT NULL," \
                    "STATUS   INTEGER          DEFAULT 0);";
    
  char * task = "CREATE TABLE IF NOT EXISTS TASK ("  \
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," \
                    "TIMEID   INTEGER          NOT NULL," \
                    "START    datetime         NOT NULL," \
                    "STOP     datetime," \
                    "FOREIGN KEY(TIMEID) REFERENCES TIME(ID));";


   sqlQuery(db, callback, time);    
   sqlQuery(db, callback, task);
   foreignKey(db, 1);

}


void deleteTask(sqlite3 * db, fC callback, int id){    
   /*
    * int flag:
    * 0 - remove row
    * 1 - remove all
    */ 
    
   foreignKey(db, 0);

   if(id && id > 0)
        sqlQuery(db, callback, "DELETE FROM TIME WHERE ID = %d; DELETE FROM TASK WHERE TIMEID = %d;", id, id);

    else if(id == -1)
        sqlQuery(db, callback, "DELETE FROM TIME; DELETE FROM TASK;");    

   foreignKey(db, 1);
}


void updateStatus(sqlite3 * db, fC callback, int id, int status){
    
    /*
     * int status:
     * status == 1 - active
     * status == 0 - no active
     */

    char * query = status ? "UPDATE TIME SET STATUS = 1 WHERE ID = %d;"
                          : "UPDATE TIME SET STATUS = 0 WHERE ID = %d;";
    
    sqlQuery(db, callback, query, id); 
}

void startStop(sqlite3 * db, fC callback, int id){
    
    values * val = selectFromTable(db, "select count(*) as count from TASK where TIMEID = %d AND STOP IS NULL;", id);
    
    int flag = atoi(val->result[1]);

    updateStatus(db, callback, id, !flag);

    /*
     * int flag:
     * flag == FALSE - INSERT
     * flag == TRUE  - UPDATE
     */ 
    
    if(!flag) sqlQuery(db, callback, "INSERT INTO TASK (TIMEID, START) VALUES (%d, datetime('now'));", id);
    else sqlQuery(db, callback, "UPDATE TASK SET STOP = datetime('now') WHERE TIMEID = %d AND STOP IS NULL;", id);

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


double getTaskTime(sqlite3 * db, char * sql, int id){
    int i, j, proxy;
    double seconds = 0.0;
    
    values * val = selectFromTable(db, sql, id);
        
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

void printTable(sqlite3 * db, char * sql, int id){
    int i, j, timeId;
    double seconds = 0.0;
    values * val;

    if(id > 0) {
        val = selectFromTable(db, sql, id);
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
            seconds = getTaskTime(db, "select START, STOP from TASK where TIMEID = %d;", timeId);
            cl hms = secToTime(seconds);
            printf("%.2d:%.2d:%.2d\t", hms.hours, hms.min, hms.sec);
            printf("%s\t", atoi(val->result[i]) ? "Start" : "Pause");
            j++;
        }
    }

    freeStructValues(val);   
}
