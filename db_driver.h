#ifndef DB_DRIVER
#define DB_DRIVER


#include <sqlite3.h>

// int init_db(sqlite3 **);
int init_db(const char * filename, sqlite3 **db);
int init_db_tables(sqlite3 *);
int db_close(sqlite3 *);

#endif