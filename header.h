#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h> 

extern void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **));
extern void initTables(sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void addDoing(char * msg, sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void selectAll(sqlite3 * db, int (* callback)(void *, int, char **, char **));
extern void deleteTask(int id, sqlite3 * db, int (* callback)(void *, int, char **, char **))

#endif // HEADER

