#ifndef HEADER
#define HEADER
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

extern void sqlQuery(sqlite3 * db, const char * sql, int (* callback)(void *, int, char **, char **));

#endif // HEADER

