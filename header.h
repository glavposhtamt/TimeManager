#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "wrapper.h"

/* Commands */

#define ADD           "add"
#define REMOVE        "rm"
#define DISPLAY       "disp"
#define UNDISPLAY     "undisp"
#define SHOW_ALL      "all"
#define INFO          "info"

/* Flags */

#define ADD_GROUP "-g"
#define ADD_TASK  "-t"

/* fC is int (* fC)(void *, int, char **, char **) */

typedef struct Clock {
    int sec;
    int min;
    int hours;
} cl;

extern void initTables(sqlite3 * db, fC);
extern void deleteTask(sqlite3 * db, fC, int);
extern void deleteGroup(sqlite3 * db, fC, int);
extern void startStop(sqlite3 * db, fC, int);
extern void printTable(sqlite3 * db, char *, int);
extern cl secToTime(double seconds);

#endif // HEADER

