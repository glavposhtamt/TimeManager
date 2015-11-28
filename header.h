#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stddef.h>
#include <time.h>
#include <stdarg.h>

/* Commands */

#define MESSAGE       "add"
#define REMOVE        "rm"
#define CLEAN         "clean"
#define LAST_DAY      "ld"
#define SHOW_ALL      "all"
#define CHANGE        "ch"
#define INFO          "info"

extern void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void sqlQuery(sqlite3 * db, int (* callback)(void *, int, char **, char **), char * fmt, ... );
extern void deleteTask(sqlite3 * db, int (* callback)(void *, int, char **, char **), int);
extern void startStop(sqlite3 * db, int (* callback)(void *, int, char **, char **), int id);
extern void printTable(sqlite3 * db, char *, int);
extern int callback(void * data, int argc, char **argv, char ** azColName);

typedef struct Query {
   char ** result;
   int columns;
   int rows;
} values;

typedef struct Clock {
    int sec;
    int min;
    int hours;
} cl;

#endif // HEADER

