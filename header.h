#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "wrapper.h"

/* Non meta columns count (first columns) */
#define DISPLAY_FIRST_COLUMNS 5

/* Commands */

#define ADD           "add"
#define REMOVE        "rm"
#define DISPLAY       "disp"
#define UNDISPLAY     "undisp"
#define SHOW_ALL      "all"
#define INFO          "info"
#define INIT          "init"
#define GROUP         "gr"
#define TARGET        "tg"

/* Flags */

#define ADD_GROUP    "-group"
#define ADD_TASK     "-task"
#define ADD_TARGET   "-target"

typedef struct Clock {
    int sec;
    int min;
    int hours;
} cl;

typedef struct TimeModel {
    int id;
    int status;
    double seconds;
    char * date;
    char * task;
    
    // struct values pointer (init source)
    values * _v;
} tmodel;

/* Is not  */


/* fC is pointer: int (* fC)(void *, int, char **, char **) */

extern void initTables(sqlite3 * db, fC);
extern void deleteTask(sqlite3 * db, fC, int);
extern void deleteGroup(sqlite3 * db, fC, int);
extern void startStop(sqlite3 * db, fC, int);
extern void printTableTask(sqlite3 * db, char *, int);
extern void printTableGroup(sqlite3 * db, char *, int);
extern cl secToTime(double seconds);
extern void printTableTarget(sqlite3 * db, char *, int);

#endif // HEADER
