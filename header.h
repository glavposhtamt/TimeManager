#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <stddef.h>
#include <time.h>

#define STRUCT_COUNT 24

extern void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **), void *);
extern void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void selectAll(int flag, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **), int);
extern void updateStatusTask(int id, int status, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void taskToLastday(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void getTaskTime(char * sql, int id, sqlite3 * db);
extern void insertTimeRange( int id, sqlite3 * db, int (* callback)(void *, int, char **, char **));

typedef struct Query {
   char ** result;
   int columns;
   int rows;
} values;

#endif // HEADER

