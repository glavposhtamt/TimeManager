#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stddef.h>
#include <time.h>

/* Columns before processing */

#define START_COLUMN_NUMBER 3
#define STOP_COLUMN_NUMBER  4

/* Commands */

#define MESSAGE  "add"
#define REMOVE   "rm"
#define CLEAN    "clean"
#define LAST_DAY "ld"
#define SHOW_ALL "all"

extern void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int);
extern void updateStatusTask(int id, int status, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void taskToLastday(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void insertTimeRange( int id, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void printTable(char * sql, int id, sqlite3 * db);

typedef struct Query {
   char ** result;
   int columns;
   int rows;
} values;

#endif // HEADER

