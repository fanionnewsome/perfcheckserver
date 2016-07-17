#pragma once

#include "sqlite3.h"
#include <iostream>

using namespace std;

class Database
{
public:
	Database(std::string filename);
	~Database();

	bool openDatabase(std::string filename);
	int executeQuery(std::string query);
	void displayTable(std::string tableName);
	static int callback(void *notUsed, int argc, char **argv, char **columnName);

private:
	sqlite3	*_connection;
	std::string  _filename;
	char *_errorMessage;
	int _status;
};

