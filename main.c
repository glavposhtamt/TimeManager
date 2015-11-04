/* Compile flag -lsqlite3 */
/* cc -o time main.c function.c -g -Wall -lsqlite3 */

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

int main(int argc, char * argv[])
{
   sqlite3 *db;
   int rc;

   /* Open database */
   rc = sqlite3_open("time.db", &db);
   if( rc ){
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      exit(0);
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }
    
    initTables(db, callback);
    
    if(argc > 1) addDoing(argv[1], db, callback);
    else printf("Введи аргументы!\n");
    
    sqlite3_close(db);
   /* selectAll(db, callback);*/

    
    
/*    sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
         "SELECT * from COMPANY";*/
    
/*    sql = "DELETE from COMPANY where ID=2; " \
         "SELECT * from COMPANY";*/
    

   return 0;
}

