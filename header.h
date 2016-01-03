#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "wrapper.h"

/* Commands */

#define MESSAGE       "add"
#define REMOVE        "rm"
#define DISPLAY       "disp"
#define UNDISPLAY     "undisp"
#define SHOW_ALL      "all"
#define INFO          "info"

/* fC is int (* fC)(void *, int, char **, char **) */

extern void initTables(sqlite3 * db, fC);
extern void deleteTask(sqlite3 * db, fC, int);
extern void startStop(sqlite3 * db, fC, int id);
extern void printTable(sqlite3 * db, char *, int);

typedef struct Clock {
    int sec;
    int min;
    int hours;
} cl;

#endif // HEADER

