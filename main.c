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

    const char * info = "\nПользуйся командами:\n"\
                        "%s ... [%s, %s, %s] - Добавить задачу/группу/цель.\n"\
                        "%s [id] [%s, %s, %s] - Удалить задачу/группу/цель.\n"\
                        "%s [id] - Показать задачу.\n"\
                        "%s [id] - Скрыть задачу.\n"\
                        "%s [%s, %s, %s] - Удаление ВСЕХ задач/групп/целей.\n"\
                        "[id] - start/pause.\n"\
                        "[taskid] %s [groupid] - Добавить задачу в группу задач.\n"\
                        "%s - Вывести все задачи за все дни.\n"\
                        "%s - Вывести это сообщение.\n"\
                        "%s - Показать группы.\n"\
                        "%s - Показать цели.\n"\
                        "%s [groupid] - Отобразить список задач в группе.\n\n";

    /* Commands */   

    if(argc == 4) {
        if(!strcmp(ADD, argv[1])) {
            char * table;
            if(!strcmp(ADD_TASK, argv[3])) table = "TIME";
            else if(!strcmp(ADD_GROUP, argv[3])) table = "GROUPLIST";
            else if(!strcmp(ADD_TARGET, argv[3])){
                sqlQuery(db, callback, "INSERT INTO TARGET (MESSAGE, DATE) VALUES ('%s', datetime('now'));", argv[2]);
                sqlite3_close(db);
                return 0;
            }
            else {
               sqlite3_close(db);
               return 0;
            }
            sqlQuery(db, callback, "INSERT INTO %s (DISPLAY, MESSAGE) VALUES (1, '%s');", table, argv[2] );
        }

        else if(atoi(argv[1]) && atoi(argv[3]) && !strcmp(ADD, argv[2])){
            sqlQuery(db, callback, "UPDATE TIME SET GROUPID = %d WHERE ID = %d;",
                     atoi(argv[3]), atoi(argv[1]) );
        }

        else if(!strcmp(REMOVE, argv[1]) && atoi(argv[2])){
            if(!strcmp(ADD_TASK, argv[3])) deleteTask(db, callback, atoi(argv[2]));
            else if(!strcmp(ADD_GROUP, argv[3])) deleteGroup(db, callback, atoi(argv[2]));
            else if(!strcmp(ADD_TARGET, argv[3]))
                sqlQuery(db, callback, "DELETE FROM TARGET WHERE ID = %d;", atoi(argv[2]));
        }
        else printf("Неверно введена команда! Введи ключ info для справки.\n");
    }

    else if(argc == 3){

        if(!strcmp(DISPLAY, argv[1]))
            sqlQuery(db, callback, "UPDATE TIME SET DISPLAY = 1 WHERE ID = %d;", atoi(argv[2]));

        else if(!strcmp(UNDISPLAY, argv[1]))
            sqlQuery(db, callback, "UPDATE TIME SET DISPLAY = 0 WHERE ID = %d;", atoi(argv[2]));

        else if(!strcmp(GROUP, argv[1]))
            printTableTask(db, "SELECT ID, STATUS, MESSAGE FROM TIME WHERE GROUPID = %d;", atoi(argv[2]));

        else if(!strcmp(REMOVE, argv[1])) {
            if(!strcmp(ADD_TASK, argv[2])) deleteTask(db, callback, -1);
            else if(!strcmp(ADD_GROUP, argv[2])) deleteGroup(db, callback, -1);
            else if(!strcmp(ADD_TARGET, argv[2])) sqlQuery(db, callback, "DELETE FROM TARGET;");
        }
            
        else printf("Неверно введена команда! Введи ключ info для справки.\n");
    }
    else if(argc == 2) { 
        if(atoi(argv[1]) > 0) startStop(db, callback, atoi(argv[1]));
        else if(!strcmp(SHOW_ALL, argv[1])) printTableTask(db, "SELECT ID, STATUS, MESSAGE FROM TIME;", 0);
        else if(!strcmp(UNDISPLAY, argv[1])) sqlQuery(db, callback, "UPDATE TIME SET DISPLAY = 0;");
        else if(!strcmp(DISPLAY, argv[1])) sqlQuery(db, callback, "UPDATE TIME SET DISPLAY = 1;");
        else  if(!strcmp(GROUP, argv[1])) printTableGroup(db, "SELECT ID, MESSAGE FROM grouplist WHERE display = 1", 0);
        else  if(!strcmp(TARGET, argv[1])) printTableTarget(db, "SELECT ID, DATE, MESSAGE FROM TARGET", 0);

        else if(!strcmp(INFO, argv[1])) 
             printf(info, ADD, ADD_TASK, ADD_GROUP, ADD_TARGET,
                         REMOVE, ADD_TASK, ADD_GROUP, ADD_TARGET,
                         DISPLAY, UNDISPLAY,
                         REMOVE, ADD_TASK, ADD_GROUP, ADD_TARGET,
                         ADD, SHOW_ALL, INFO, GROUP, TARGET, GROUP);
        
        else printf("Неверно введена команда! Введи ключ info для справки.\n");
    } 
    else if(argc == 1) printTableTask(db, "SELECT ID, STATUS, MESSAGE FROM TIME WHERE DISPLAY = 1;", 0);
    else printf("Неверно введена команда! Введи ключ info для справки.\n");

                     
   /* Close database */

    sqlite3_close(db);
    
   return 0;
}

