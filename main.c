#include "header.h"

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
                        "%s - Удаление ВСЕХ задач\n"\
                        "[id] - start/pause.\n"\
                        "%s - Вывести все задачи за все дни.\n"\
                        "%s - Вывести это сообщение\n";

    /* Commands */   

    if(argc == 3){
        if(!strcmp(MESSAGE, argv[1])) 
            sqlQuery(db, callback, "INSERT INTO TIME (DATE, MESSAGE) VALUES (date('now'), '%s');", argv[2] );
        
        else if(!strcmp(REMOVE, argv[1])) deleteTask(db, callback, atoi(argv[2]));

        else if(!strcmp(LAST_DAY, argv[1]))
            sqlQuery(db, callback, "UPDATE TIME SET DATE = date('now') WHERE ID = %d;", atoi(argv[2]));
            
        else printf("Неверно введена команда!\n");
    }
    else if(argc == 2) { 
        if(atoi(argv[1]) > 0) startStop(db, callback, atoi(argv[1]));
        else if(!strcmp(CLEAN, argv[1])) deleteTask(db, callback, -1);
        else if(!strcmp(SHOW_ALL, argv[1])) printTable(db, "select ID, STATUS, MESSAGE from TIME;", 0);
        else if(!strcmp(INFO, argv[1])) 
            printf(info, MESSAGE, REMOVE, LAST_DAY, CLEAN, SHOW_ALL, INFO);
        
        else printf("Неверно введена команда!\n");
    } 
    else if(argc == 1) printTable(db, "select ID, STATUS, MESSAGE from TIME WHERE date = date('now');", 0);
    else printf(info, MESSAGE, REMOVE, LAST_DAY, CLEAN, SHOW_ALL, INFO);
                     
   /* Close database */

    sqlite3_close(db);
    
   return 0;
}

