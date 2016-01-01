#ifndef WRAPPER
#define WRAPPER

/* SQLite3 C - library wrapper */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <stddef.h>
#include <stdarg.h>

typedef struct Query {
   char ** result;
   int columns;
   int rows;
} values;

typedef int (* fC)(void *, int, char **, char **);

extern int callback(void * data, int argc, char **argv, char ** azColName);
extern void sqlQuery(sqlite3 * db, fC, char * fmt, ... );
extern values * selectFromTable(sqlite3 * db, char * fmt, ...);
extern void freeStructValues(values * val);
extern void foreignKey(sqlite3 * db, int flag);

#endif // WRAPPER

