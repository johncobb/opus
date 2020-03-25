
#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "db_driver.h"

using namespace std;


int init_db(const char * filename, sqlite3 **db) {
	return sqlite3_open(filename, db);
}

int db_close(sqlite3 *db) {
	sqlite3_close(db);
}

static int callback(void *notused, int argc, char **argv, char ** azcolname) {
	return 0;
}

int init_db_tables(sqlite3 *db) {

	char *errmsg = 0;
	int rc;
	char *sql;

	rc = sqlite3_open("test.db", &db);

	sql = "CREATE TABLE TaskMeta(Id INT NOT NULL, Name CHAR(50) NOT NULL);";
	// sql = "CREATE TABLE TaskJournal (Id, INT NOT NULL);";

	/* execute sql statement */
	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);
	sql = "INSERT INTO TaskMeta VALUES(256, 'Task 1');" \
		  "INSERT INTO TaskMeta VALUES(512, 'Task 2');" \
		  "INSERT INTO TaskMeta VALUES(1024, 'Task 3');" \
		  "INSERT INTO TaskMeta VALUES(2048, 'Task 4');";

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);

	if (rc != SQLITE_OK) {
		cout << stderr << " SQL error: " << errmsg << endl;
	} else {
		cout << "table created successfully" << endl;
	}

	return 0;
}

