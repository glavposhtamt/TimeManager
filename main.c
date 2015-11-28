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

    /* Info */

    const char * info = "Пользуйся командами:\n"\
                        "%s ... - Добавить задачу.\n"\
                        "%s [id] - Удалить задачу.\n"\
                        "%s [id] - Перенести на следующий день.\n"\
                        "%s [id] - start/pause.\n"\
                        "%s - Удаление ВСЕХ задач\n"\
                        "%s - Вывести все задачи за все дни.\n"\
                        "%s - Вывести это сообщение\n"\
                        "%s %s ... - Добавить цель.\n"\
                        "%s %s [id] - Удалить цель.\n"\
                        "%s %s - Удаление ВСЕХ целей\n";
    /* Commands */   
    
    if(argc == 4){
        if(!strcmp(MESSAGE, argv[1]) && !strcmp(TARGET, argv[2])) addDoing(argv[3], "TARGET", db, callback);
        else if(!strcmp(REMOVE, argv[1]) && !strcmp(TARGET, argv[2])) deleteTarget(atoi(argv[3]), db, callback, 0);
        else printf("Неверно введена команда!\n");
    }
    else if(argc == 3){
        if(!strcmp(MESSAGE, argv[1])) addDoing(argv[2], "TIME", db, callback);
        else if(!strcmp(REMOVE, argv[1])) deleteTask(atoi(argv[2]), db, callback, 0);
        else if(!strcmp(LAST_DAY, argv[1])) taskToLastday(atoi(argv[2]), db, callback);
        else if(!strcmp(CHANGE, argv[1])) startStop(atoi(argv[2]), db, callback);
        if(!strcmp(CLEAN, argv[1]) && !strcmp(TARGET, argv[2])) deleteTarget(0, db, callback, 1);
        else printf("Неверно введена команда!\n");
    }
    else if(argc == 2) { 
        if(!strcmp(CLEAN, argv[1])) deleteTask(0, db, callback, 1);
        else if(!strcmp(SHOW_ALL, argv[1])) printTable("select ID, STATUS, MESSAGE from TIME;", 0, db);
        else if(!strcmp(INFO, argv[1])) 
            printf(info, MESSAGE, REMOVE, LAST_DAY, CHANGE, CLEAN, SHOW_ALL, INFO, MESSAGE, TARGET, REMOVE, TARGET, CLEAN, TARGET);
        else printf("Неверно введена команда!\n");
    } 
    else if(argc == 1) printTable("select ID, STATUS, MESSAGE from TIME WHERE date = date('now');", 0, db);
    else printf(info, MESSAGE, REMOVE, LAST_DAY, CHANGE, CLEAN, SHOW_ALL, INFO, MESSAGE, TARGET, REMOVE, TARGET, CLEAN, TARGET);

   /* Close database */

    sqlite3_close(db);
    
   return 0;
}

