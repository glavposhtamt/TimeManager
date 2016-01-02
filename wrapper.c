#include "wrapper.h"

int callback(void * data, int argc, char **argv, char ** azColName){ return 0; }

void sqlQuery(sqlite3 * db, fC callback, char * fmt, ... ){
    
   va_list ap;
   int d, rc, count = 0, i = 0;
   char c, *s, *zErrMsg = 0, query[256], proxy[265], *sql = fmt;

   va_start(ap, fmt);
   while (*fmt) {
       query[i++] = *fmt;

       if(*fmt++ == '%') {
           switch (*fmt) {
               case 's':              /* string */
                   query[i++] = 's';
                   query[i] = '\0';

                   s = va_arg(ap, char *);

                   sprintf(proxy, query, s);

                   for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                   query[i] = '\0';
                   break;

               case 'd':              /* int */
                   query[i++] = 'd';
                   query[i] = '\0';

                   d = va_arg(ap, int);

                   sprintf(proxy, query, d);

                   for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                   query[i] = '\0';
                   break;

               case 'c':              /* char */
                   query[i++] = 'c';
                   query[i] = '\0';

                   c = (char) va_arg(ap, int);

                   sprintf(proxy, query, c);

                   for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                   query[i] = '\0';
                   break;
            }

           ++count;
       }
   }

    
   va_end(ap);

   if(count) rc = sqlite3_exec(db, query, callback, NULL, &zErrMsg); 
   else rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);
    
   if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

values * selectFromTable(sqlite3 * db, char * fmt, ...){
    va_list ap;
    int rc, nrows, ncols, d, count = 0, i = 0;
    char ** result, * errmsg, c, *s, query[256], proxy[265], *sql = fmt;
    values * val = malloc(sizeof(values));
    
    va_start(ap, fmt);
    while (*fmt) {
        query[i++] = *fmt;

        if(*fmt++ == '%') {
            switch (*fmt) {
                case 's':              /* string */
                    query[i++] = 's';
                    query[i] = '\0';

                    s = va_arg(ap, char *);

                    sprintf(proxy, query, s);

                    for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                    query[i] = '\0';
                    break;

                case 'd':              /* int */
                    query[i++] = 'd';
                    query[i] = '\0';

                    d = va_arg(ap, int);

                    sprintf(proxy, query, d);

                    for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                    query[i] = '\0';
                    printf("%s\n", query);
                    break;

                case 'c':              /* char */
                    query[i++] = 'c';
                    query[i] = '\0';

                    c = (char) va_arg(ap, int);

                    sprintf(proxy, query, c);

                    for(i = 0; proxy[i] != '\0'; i++) query[i] = proxy[i];
                    query[i] = '\0';
                    break;
             }

            ++count;
        }
    }


    va_end(ap);


    if(count) rc = sqlite3_get_table(db, query, &result, &nrows, &ncols, &errmsg);
    else rc = sqlite3_get_table(db, sql, &result, &nrows, &ncols, &errmsg);
    
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_free_table(result);
        exit(1);
    }else{
        
        val->result = result;
        val->columns = ncols;
        val->rows = nrows;
        
        return val;        
    }    
}

void foreignKey(sqlite3 * db, int flag){
    char * foreignKeyOn = "PRAGMA foreign_keys = ON;";
    char * foreignKeyOff = "PRAGMA foreign_keys = OFF;";

    if(flag) sqlQuery(db, callback, foreignKeyOn);
    else sqlQuery(db, callback, foreignKeyOff);
}

void freeStructValues(values * val){
    sqlite3_free_table(val->result);
    free(val);
}
