#include "header.h"

void initTables(sqlite3 * db, fC callback)
{
    char * group = "CREATE TABLE IF NOT EXISTS GROUPLIST ("
                      "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," 
                      "DISPLAY  INTEGER          NOT NULL,"
                      "MESSAGE  CHAR(100)        NOT NULL);";

    char * time = "CREATE TABLE IF NOT EXISTS TIME ("
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                    "DATE     date             NOT NULL,"
                    "MESSAGE  CHAR(100)        NOT NULL," 
                    "DISPLAY  INTEGER          NOT NULL,"
                    "GROUPID  INTEGER          DEFAULT 0,"
                    "STATUS   INTEGER          DEFAULT 0);";
    
    char * task = "CREATE TABLE IF NOT EXISTS TASK ("
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                    "TIMEID   INTEGER          NOT NULL,"
                    "START    datetime         NOT NULL,"
                    "STOP     datetime,"
                    "FOREIGN KEY(TIMEID) REFERENCES TIME(ID));";

    char * target = "CREATE TABLE IF NOT EXISTS TARGET ("
                    "ID       INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                    "MESSAGE  CHAR(100)        NOT NULL,"
                    "DATE     datetime         NOT NULL);";

    QUERY(db, callback, "%s", time);    
    QUERY(db, callback, "%s", task);
    QUERY(db, callback, "%s", group);
    QUERY(db, callback, "%s", target);
}


void deleteTask(sqlite3 * db, fC callback, int id){    
   /*
    * int id:
    * 0 - remove row
    * 1 - remove all
    */ 
    
    if (id && id > 0)
    {
        QUERY(db, callback, "DELETE FROM TIME WHERE ID = %d; DELETE FROM TASK WHERE TIMEID = %d;", id, id);
    }

    else if(id == -1)
    {
        QUERY(db, callback, "DELETE FROM TIME; DELETE FROM TASK;");
    }  

}

void deleteGroup(sqlite3 * db, fC callback, int id)
{
   /*
    * int id:
    * 0 - remove row
    * 1 - remove all
    */

    if (id && id > 0)
    {
        QUERY(db, callback, "DELETE FROM GROUPLIST WHERE ID = %d;", id);
    }

    else if (id == -1)
    {
        QUERY(db, callback, "DELETE FROM GROUPLIST;");
    }
}

void updateStatus(sqlite3 * db, fC callback, int id, int status)
{
    
    /*
     * int status:
     * status == 1 - active
     * status == 0 - no active
     */

    char * query = status ? "UPDATE TIME SET STATUS = 1 WHERE ID = %d;"
                          : "UPDATE TIME SET STATUS = 0 WHERE ID = %d;";

    QUERY(db, callback, query, id);
}

void startStop(sqlite3 * db, fC callback, int id)
{ 
    values * val = malloc(sizeof(values));
    QUERY_SELECT(val, db, "select count(*) as count from TASK where TIMEID = %d AND STOP IS NULL;", id);
    
    int flag = atoi(val->result[1]);

    updateStatus(db, callback, id, !flag);

    /*
     * int flag:
     * flag == FALSE - INSERT
     * flag == TRUE  - UPDATE
     */ 

    foreignKey(db, 1);
    
    if(!flag)
    {
        QUERY(db, callback, "INSERT INTO TASK (TIMEID, START) VALUES (%d, datetime('now'));", id);
    }
    else
    {
        QUERY(db, callback, "UPDATE TASK SET STOP = datetime('now') WHERE TIMEID = %d AND STOP IS NULL;", id);
    }

    foreignKey(db, 0);

    freeStructValues(val);
}

double getPeriod(char * dateStart, char * dateStop)
{
    struct tm tmStart = { 0 }, 
              tmStop = { 0 };

    time_t timeStampStart, timeStampStop;
        
    strptime(dateStart, "%Y-%m-%d %H:%M:%S", &tmStart);
    strptime(dateStop, "%Y-%m-%d %H:%M:%S", &tmStop);

    timeStampStart = mktime(&tmStart);
    timeStampStop = mktime(&tmStop);
        
    return difftime(timeStampStop, timeStampStart);
}

tmodel ** initTmodel(sqlite3 * db, int timeid)
{
    int i, j, columnN, uid = 0, id;
    char * start;
    values * val = malloc(sizeof(values));

    const char * sql =  "SELECT time.id, status, date, message, start, stop "
                        "FROM time LEFT JOIN task ON task.timeid = time.id ORDER BY time.id;";

    // Init values struct
    if (timeid > 0)
    {
        QUERY_SELECT(val, db, sql, timeid);
    }
    else
    {
        QUERY_SELECT(val, db, "%s", sql);
    }

    // Init tmodel struct
    tmodel ** aTmodel = malloc(sizeof(tmodel *) * val->rows);
    int count = val->columns * val->rows + val->columns;
    
    for (i = val->columns, j = 0, columnN = 1; i < count; i++) {

        switch (columnN)
        {
            // ID
            case 1:
                id = atoi(val->result[i]);
                if (!uid || id != uid)
                {
                    j = uid ? j + 1 : 0;
                    aTmodel[j] = malloc(sizeof(tmodel));
                    aTmodel[j]->_v = val;
                    aTmodel[j]->id = id;
                    uid = id;
                }
                if (id == (count - val->columns))
                {
                    aTmodel[j]->id = id;
                }

                columnN++;
                continue;

            // STATUS
            case 2:
                aTmodel[j]->status = atoi(val->result[i]);
                columnN++;
                continue;

            // DATE
            case 3:
                aTmodel[j]->date = val->result[i];
                columnN++;
                continue;
            
            // TASK
            case 4: 
                aTmodel[j]->task = val->result[i];
                columnN++;
                continue;

            // SECONDS
            case 5: 
                start = val->result[i];
                ++i;
                if (val->result[i] != NULL)
                {
                    aTmodel[j]->seconds += getPeriod(start, val->result[i]);
                }
                columnN = 1;
                continue;
        }
    }
    
    // Last init after for
    if (j)
    {
        aTmodel[++j] = NULL;
    }

    return aTmodel;
}

void freeStructTmodel(tmodel ** tmodel)
{
    int i = 0;
    
    freeStructValues((*tmodel)->_v);

    while(tmodel[i])
    {
        free(tmodel[i]);
        ++i;
    }

    free(tmodel);
}

double getTaskTime(sqlite3 * db, char * sql, int id)
{
    int i, j, proxy;
    double seconds = 0.0;
    
    values * val = malloc(sizeof(values));

    QUERY_SELECT(val, db, sql, id);
        
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

cl secToTime(double seconds)
{
    long s = (long)seconds;
    int allMin;
    cl hms = { 0 };

    hms.sec = s >= 60 ? s % 60 : s;
    allMin = s / 60;
    hms.min = allMin >= 60 ? allMin % 60 : allMin;
    hms.hours = allMin / 60;

    return hms;
}

int secToDays(double seconds)
{
    return (int)seconds / 60 / 60 / 24;
}

void printTableTask(sqlite3 * db, char * sql, int id)
{
    tmodel ** tmodel = initTmodel(db, 0);
    int i = 0;
    cl hms = { 0 };

    while(tmodel[i])
    {
        hms = secToTime(tmodel[i]->seconds);

        printf("[%d]\t%.2d:%.2d:%.2d\t%s\t%s\t%s\n", 
            tmodel[i]->id, hms.hours, hms.min, hms.sec, 
            tmodel[i]->status ? "Start" : "Pause", tmodel[i]->date, tmodel[i]->task);

        ++i;
    }

    freeStructTmodel(tmodel);
}

void printTableGroup(sqlite3 * db, char * sql, int id){
    int i, j, k, timeId;
    double seconds = .0, secondsNow = .0;
    values * val = malloc(sizeof(values)),
          * ids = malloc(sizeof(values));

    if (id > 0)
    {
        QUERY_SELECT(val, db, sql, id);
    }
    else
    {
        QUERY_SELECT(val, db, "%s", sql);
    }

    int count = val->columns * val->rows + val->columns;

    for (i = val->columns, j = 1; i < count; i++) {
        switch(j) {
            case 1:
                timeId = atoi(val->result[i]);
                printf("[%d]\t", timeId);

                QUERY_SELECT(ids, db, "SELECT id FROM time WHERE groupid = %d", timeId);
                int tcount = ids->columns * ids->rows + ids->columns;

                for (k = ids->columns; k < tcount; k++) {
                    seconds += getTaskTime(db, "select START, STOP from TASK where TIMEID = %d;", atoi(ids->result[k]));
                    secondsNow += getTaskTime(db, "select START, STOP from TASK where TIMEID = %d AND strftime('%%Y-%%m-%%d', STOP) = date('now');",
                                              atoi(ids->result[k]));
                }

                freeStructValues(ids);

                j++;
                break;

            case 3:
                printf("%s\n", val->result[i]);
                j = 1;
                break;
       }
        if (j == 2) {
            cl hms = secToTime(seconds);
            cl hmsNow = secToTime(secondsNow);

            printf("%.2d:%.2d:%.2d\t", hms.hours, hms.min, hms.sec);
            printf("%.2d:%.2d:%.2d\t", hmsNow.hours, hmsNow.min, hmsNow.sec);

            seconds = 0.0;
            secondsNow = 0.0;
            j++;
        }

    }

    freeStructValues(val);
}

void printTableTarget(sqlite3 * db, char * sql, int id)
{
    int i, j;
    double seconds;
    char curentTime[20];
    values * val = malloc(sizeof(values));
    time_t tp;

    time(&tp);
    struct tm * mytm = gmtime(&tp);

    //mytm->tm_hour += 3;
    strftime(curentTime, (size_t)20, "%Y-%m-%d %H:%M:%S", mytm);

    if (id > 0) 
    {
        QUERY_SELECT(val, db, sql, id);
    }
    else
    {
        QUERY_SELECT(val, db, "%s", sql);
    }

    int count = val->columns * val->rows + val->columns;

    for (i = val->columns, j = 1; i < count; i++) {
        switch (j)
        {
            case 1:
                printf("[%d]\t", atoi(val->result[i]));
                j++;
                break;

            case 2:
                seconds = getPeriod(val->result[i], curentTime);
                printf("%s\t", val->result[i]);
                printf("%d\t", secToDays(seconds));
                j++;
                break;

            case 3:
                printf("%s\n", val->result[i]);
                j = 1;
                break;
        }
    }

    freeStructValues(val);

}