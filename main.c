#include "header.h"

int main(int argc, char * argv[])
{
    sqlite3 * db;
    int rc;

    /* Open database */
    rc = sqlite3_open("time.db", &db);

    if(rc)
    {
       fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
       exit(0);
    }

   printTable(getAllTasks(db));
                     
   /* Close database */
    sqlite3_close(db);

   return 0;
}

