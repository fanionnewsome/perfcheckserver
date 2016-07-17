#include "stdafx.h"
#include "Database.h"



Database::Database(std::string filename): _filename(filename)
{
	
}


Database::~Database()
{
	if (_connection) {
		sqlite3_close(_connection);
	}
}

bool Database::openDatabase(std::string filename) {
	this->_status = sqlite3_open(filename.c_str(), &this->_connection);

	if( this->_status )
	{
		std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(_connection) << std::endl << std::endl;
		sqlite3_close(_connection);
		return 1;
	}
	else {
		cout << "Opened " << filename.c_str() << " successfully" << std::endl;
	}
}

int Database::executeQuery(std::string query) {

	this->_status = sqlite3_exec(this->_connection, query.c_str(), callback, 0, &this->_errorMessage);
    
	if (this->_status != SQLITE_OK) {
		fprintf(stderr, "SQL Error: %s\n", this->_errorMessage);
		sqlite3_free(this->_errorMessage);
	}

	return this->_status;
}

int Database::callback(void *notUsed, int argc, char **argv, char **columnName) {

	for (int i = 0; i < argc; ++i) {
		printf("%s = %s\n", columnName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
