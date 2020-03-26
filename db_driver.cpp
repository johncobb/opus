
#include <iostream>
#include <cstdint>
#include <stdio.h>
#include <stdlib.h>
#include "db_driver.h"

// using namespace std;


char sql_buffer[512] = {0};


int db_init(const char *filename, sqlite3 **db) {
	return sqlite3_open(filename, db);
}

int db_close(sqlite3 *db) {
	sqlite3_close(db);
}

static int callback(void *notused, int argc, char **argv, char ** azcolname) {
	return 0;
}

int db_init_schema(const char *filename, sqlite3 *db) {

	char *errmsg = 0;
	int rc;
	char *sql;

	/*
	 * TODO: research opening database before querys. currently
	 * we only open the database on schema initialization. refactoring
	 * could pose problems in the future.
	 */
	rc = sqlite3_open(filename, &db);

	/*
	 * Table: TaskMeta
	 * Id - Task Identifier (identified by 8 high order bits)
	 * SubTasks - Number of subtasks represented by 8 low order bits
	 * Name - Task Name
	 */
	sql = "CREATE TABLE TaskMeta(Id INT NOT NULL, SubTasks INT NOT NULL, Name CHAR(50) NOT NULL);";

	/* execute sql statement */
	rc = db_run_query(db, sql);

	if (rc == SQLITE_OK) {
		std::cout << "table TaskMeta created successfully." << std::endl;
	}
	/*
	 * Table: TaskJournal
	 * TaskId - current state of task
	 * Epoch - epoch timestamp
	 */
	sql = "CREATE TABLE TaskJournal (TaskId INT NOT NULL, Epoch INT NOT NULL)";

	/* execute sql statement */
	rc = db_run_query(db, sql);

	if (rc == SQLITE_OK) {
		std::cout << "table TaskJournal created successfully." << std::endl;
	}

	return 0;
}


int db_run_query(sqlite3 *db, const char *sql) {
	char *errmsg = 0;
	int rc;

	rc = sqlite3_exec(db, sql, callback, 0, &errmsg);

	if (rc != SQLITE_OK) {
		std::cout << stderr << " SQL error: " << errmsg << std::endl;
	} else {
		std::cout << "Query: " << sql << std::endl << "successful." << std::endl;
	}

	return rc;

}

