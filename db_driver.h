#ifndef DB_DRIVER
#define DB_DRIVER


#include <sqlite3.h>

// int init_db(sqlite3 **);
int db_init(const char * filename, sqlite3 **db);
int db_init_schema(sqlite3 *);
int db_close(sqlite3 *);

#endif