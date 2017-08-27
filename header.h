#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "wrapper.h"

/* Commands */

#define ADD           "add"
#define REMOVE        "rm"
#define DISPLAY       "disp"
#define UNDISPLAY     "undisp"
#define SHOW_ALL      "all"
#define INFO          "info"
#define GROUP         "gr"
#define TARGET        "tg"

/* Flags */

#define ADD_GROUP    "-group"
#define ADD_TASK     "-task"
#define ADD_TARGET   "-target"

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
extern void printTableTask(sqlite3 * db, char *, int);
extern void printTableGroup(sqlite3 * db, char *, int);
extern cl secToTime(double seconds);
extern void printTableTarget(sqlite3 * db, char *, int);

#endif // HEADER
