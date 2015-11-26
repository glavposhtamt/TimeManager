/* cc -o time main.c function.c -g -Wall -lsqlite3 -D_XOPEN_SOURCE=600 */

#include "header.h"

static int callback(void * data, int argc, char **argv, char ** azColName){

   if(data != NULL) fprintf(stderr, "%s: ", (const char *)data);
   
   return 0;
}


int main(int argc, char * argv[]){
   sqlite3 * db;
   int rc;
   
   /* Open database */
    
   rc = sqlite3_open("time.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }

    initTables(db, callback);
    
    /* Commands */

    if(argc == 3){
        if(!strcmp(MESSAGE, argv[1])) addDoing(argv[2], db, callback);
        else if(!strcmp(REMOVE, argv[1])) deleteTask(atoi(argv[2]), db, callback, 0);
        else if(!strcmp(LAST_DAY, argv[1])) taskToLastday(atoi(argv[2]), db, callback);
        else printf("Неверно введена команда!\n");
    }
    else if(argc == 2) { 
        if(!strcmp(CLEAN, argv[1])) deleteTask(0, db, callback, 1);
        if(!strcmp(SHOW_ALL, argv[1])) printTable("select ID, MESSAGE from TIME;", 0, db);
        else printf("Неверно введена команда!\n");
    } 
    else printf("Пользуйся командами:...\n");

    //updateStatusTask(12, 11, db, callback);


   /* Close database */

    sqlite3_close(db);
    
   return 0;
}

