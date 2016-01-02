#ifndef GROUP
#define GROUP

#include <stdio.h>
#include <stdlib.h>

#include "wrapper.h"

/* Commands */

#define MESSAGE       "add"
#define REMOVE        "rm"
#define CLEAN         "clean"
#define DISPLAY       "disp"
#define UNDISPLAY     "undisp"
#define SHOW_ALL      "all"
#define INFO          "info"

/* fC is int (* fC)(void *, int, char **, char **) */

extern void initTables(sqlite3 * db, fC);

#endif // GROUP

