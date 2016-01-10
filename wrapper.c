#include "wrapper.h"

int callback(void * data, int argc, char **argv, char ** azColName){ return 0; }

void sqlQuery(sqlite3 * db, fC callback, char * fmt, ... ){

   va_list ap;
   int d, rc, i = 0;
   char c, *s, *zErrMsg = 0, proxy[265], buff[256];

   va_start(ap, fmt);

    while (*fmt){
        proxy[i++] = *fmt;
        if(*fmt++ == '%')
            switch (*fmt++) {
            case 's':              /* string */
                proxy[i++] = 's';
                proxy[i] = '\0';
                s = va_arg(ap, char *);
                sprintf(buff, proxy, s);
                for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                break;
            case 'd':              /* int */
                proxy[i++] = 'd';
                proxy[i] = '\0';
                d = va_arg(ap, int);
                sprintf(buff, proxy, d);
                for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];

                break;
            case 'c':              /* char */
                /* need a cast here since va_arg only
                   takes fully promoted types */
                proxy[i++] = 'c';
                proxy[i] = '\0';
                c = (char) va_arg(ap, int);
                sprintf(buff, proxy, c);
                for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                break;
            case '%':
                proxy[i++] = '%';
                proxy[i++] = *fmt++;

                break;
            }
      }

    proxy[i] = '\0';

    va_end(ap);

    rc = sqlite3_exec(db, proxy, callback, NULL, &zErrMsg);

    
   if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

values * selectFromTable(sqlite3 * db, char * fmt, ...){
    va_list ap;
    int rc, nrows, ncols, d, i = 0;
    char ** result, * errmsg, c, *s, buff[256], proxy[265];
    values * val = malloc(sizeof(values));
    
    va_start(ap, fmt);

     while (*fmt){
         proxy[i++] = *fmt;
         if(*fmt++ == '%')
             switch (*fmt++) {
             case 's':              /* string */
                 proxy[i++] = 's';
                 proxy[i] = '\0';
                 s = va_arg(ap, char *);
                 sprintf(buff, proxy, s);
                 for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                 break;
             case 'd':              /* int */
                 proxy[i++] = 'd';
                 proxy[i] = '\0';
                 d = va_arg(ap, int);
                 sprintf(buff, proxy, d);
                 for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];

                 break;
             case 'c':              /* char */
                 /* need a cast here since va_arg only
                    takes fully promoted types */
                 proxy[i++] = 'c';
                 proxy[i] = '\0';
                 c = (char) va_arg(ap, int);
                 sprintf(buff, proxy, c);
                 for(i = 0; buff[i] != '\0'; i++) proxy[i] = buff[i];
                 break;
             case '%':
                 proxy[i++] = '%';
                 proxy[i++] = *fmt++;

                 break;
             }
       }

     proxy[i] = '\0';

     va_end(ap);

     printf("%s\n", proxy);

   rc = sqlite3_get_table(db, proxy, &result, &nrows, &ncols, &errmsg);

    
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

    if(flag) sqlQuery(db, callback, "PRAGMA foreign_keys = ON;");
    else sqlQuery(db, callback, "PRAGMA foreign_keys = OFF;");
}

void freeStructValues(values * val){
    sqlite3_free_table(val->result);
    free(val);
}
