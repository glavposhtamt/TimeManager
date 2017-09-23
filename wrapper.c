#include "wrapper.h"

int callback(void * data, int argc, char **argv, char ** azColName){ return 0; }


void sqlExec(sqlite3 * db, fC callback, const char * sqlString)
{
    int rc;
    char *zErrMsg;

    rc = sqlite3_exec(db, sqlString, callback, NULL, &zErrMsg);
    
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
}

/*
 *  struct values init
 */
void sqlData(sqlite3 * db, char * sqlString, values * val)
{
    int rc, nrows, ncols;
    char ** result, * errmsg;

    rc = sqlite3_get_table(db, sqlString, &result, &nrows, &ncols, &errmsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errmsg);
        sqlite3_free(errmsg);
        sqlite3_free_table(result);
        exit(1);
    }
    else
    {   
        val->result = result;
        val->columns = ncols;
        val->rows = nrows;       
    }    
}

void foreignKey(sqlite3 * db, int flag)
{
    if(flag)
    {
        QUERY(db, callback, "PRAGMA foreign_keys = ON;");
    }
    else 
    {
        QUERY(db, callback, "PRAGMA foreign_keys = OFF;");
    }
}

void freeStructValues(values * val)
{
    sqlite3_free_table(val->result);
    free(val);
}