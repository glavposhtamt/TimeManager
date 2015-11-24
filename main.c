/* Compile flag -lsqlite3 */
/* cc -o time main.c function.c -g -Wall -lsqlite3 -D_XOPEN_SOURCE=600 */

#include "header.h"

static int callback(void * data, int argc, char **argv, char ** azColName){
   int i;
   fprintf(stderr, "%s: ", (const char *)data);
   for(i = 0; i < argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
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
    
    getTaskTime("SELECT * FROM TASK WHERE TIMEID = %d;", 1, db);
    
    if(argc > 1) addDoing(argv[1], db, callback);
    else printf("Введи аргументы!\n");

/*  deleteTask(0, db, callback, 0);
    
    updateStatusTask(12, 11, db, callback);
    
    taskToLastday(12, db, callback);
    
    selectAll(1, db, callback);*/
         
    sqlite3_close(db);
    
   return 0;
}

