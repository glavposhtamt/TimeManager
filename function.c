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

/*   @SQL:
    SELECT * FROM
      (SELECT time.id, status,
        IFNULL(
            SUM(Cast((JulianDay(stop) - JulianDay(start)) * 24 * 60 * 60 As Integer)),
            SUM(Cast((JulianDay() - JulianDay(start)) * 24 * 60 * 60 As Integer))
        ) as seconds, date, message
      FROM time INNER JOIN task ON task.timeid = time.id GROUP BY time.id
      UNION ALL
      SELECT id, status, 0 as seconds, date, message FROM time
      WHERE id NOT IN(SELECT timeid FROM task)) ORDER BY id;
 */

values * getAllTasks(sqlite3 * db)
{
    const char * sql =
        "SELECT * FROM\
        (SELECT time.id, status,\
            IFNULL(\
                SUM(Cast((JulianDay(stop) - JulianDay(start)) * 24 * 60 * 60 As Integer)),\
                SUM(Cast((JulianDay() - JulianDay(start)) * 24 * 60 * 60 As Integer))\
            ) as seconds, date, message\
        FROM time INNER JOIN task ON task.timeid = time.id GROUP BY time.id\
        UNION ALL\
        SELECT id, status, 0 as seconds, date, message FROM time\
        WHERE id NOT IN(SELECT timeid FROM task)) ORDER BY id;";

    values * val = malloc(sizeof(values));
    QUERY_SELECT(val, db, "%s", sql);

    return val;
}

void printTable(values * val)
{
    int i, columnN;
    cl hms = { 0 };

    // Init tmodel struct
    if (!val->rows)
    {
        freeStructValues(val);
        return;
    }

    int count = val->columns * val->rows + val->columns;
    
    for (i = val->columns, columnN = 1; i < count; i++) {

        switch (columnN)
        {
            // ID, "[%d]\t%.2d:%.2d:%.2d\t%s\t%s\t%s\n"
            case 1:
                printf("[%s]\t", val->result[i]);
                columnN++;
                continue;

            // STATUS
            case 2:
                printf("%s\t", atoi(val->result[i]) ? "Start" : "Pause");
                columnN++;
                continue;

            // SECONDS
            case 3:
                hms = secToTime(atof(val->result[i]));
                printf("%.2d:%.2d:%.2d\t", hms.hours, hms.min, hms.sec);
                columnN++;
                continue;

            // DATE
            case 4:
                printf("%s\t", val->result[i]);
                columnN++;
                continue;
            
            // TASK
            case 5: 
                printf("%s\n", val->result[i]);
                columnN = 1;
                continue;
        }
    }

    freeStructValues(val);
    return;
}

cl secToTime(double seconds)
{
    long s = (long)seconds;
    int allMin;
    cl hms = { 0 };

    if (!s) return hms;

    hms.sec = s >= 60 ? s % 60 : s;
    allMin = s / 60;
    hms.min = allMin >= 60 ? allMin % 60 : allMin;
    hms.hours = allMin / 60;

    return hms;
}