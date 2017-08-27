#ifndef WRAPPER
#define WRAPPER

/* SQLite3 C - library wrapper */

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <stddef.h>
#include <stdarg.h>

/* Size string buffer */

#define SIZE 256
#define QUERY(db, callback, ...) { char buff[SIZE]; sprintf(buff, __VA_ARGS__); sqlExec(db, callback, buff); }

typedef struct Query {
   char ** result;
   int columns;
   int rows;
} values;

typedef int (* fC)(void *, int, char **, char **);

extern int callback(void * data, int argc, char **argv, char ** azColName);
extern void sqlExec(sqlite3 * db, fC, const char *);
extern values * selectFromTable(sqlite3 * db, char * fmt, ...);
extern void freeStructValues(values * val);
extern void foreignKey(sqlite3 * db, int flag);

#endif // WRAPPER

