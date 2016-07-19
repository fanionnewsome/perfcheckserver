#include "stdafx.h"
#include "Database.h"
#include <ios>
#include <sstream>

Database::Database(std::string filename): _filename(filename), _databaseOpen(false)
{
	// initialize engine
	int status = 0;
	if (SQLITE_OK != (status = sqlite3_initialize()))
	{
		printf("Failed to initialize library: %d\n", status);
	}
	openDatabase(_filename);
}


Database::~Database()
{
	if (_connection) {
		sqlite3_close(_connection);
	}
}

sqlite3* Database::getConnection() const {
	return this->_connection;
}

bool Database::openDatabase(std::string filename) {
	this->_status = sqlite3_open(filename.c_str(), &this->_connection);

	if( this->_status )
	{
		std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(_connection) << std::endl << std::endl;
		sqlite3_close(_connection);
		this->_databaseOpen = false;
		return false;
	}
	else {
		cout << "Opened " << filename.c_str() << " successfully" << std::endl;
		this->_databaseOpen = true;
	}
	return true;
}

void Database::displayTable(std::string tableName) {
	
	//
	// Display the given table
	//
	std::stringstream ss;
	char **results = NULL;
	int rows, columns;
	char *error;

	ss << "SELECT * FROM " << tableName << ";";
	std::string sqlSelect(ss.str());
	int status = sqlite3_get_table(_connection, sqlSelect.c_str(), &results, &rows, &columns, &error);
	if (status)
	{
		std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(_connection) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		// display table if table exists
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// determine cell position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				std::cout.width(12);
				std::cout.setf(ios::left);
				std::cout << results[cellPosition] << " ";
			}

			// End Line
			std::cout << std::endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					std::cout.width(12);
					std::cout.setf(ios::left);
					std::cout << "~~~~~~~~~~~~ ";
				}
				std::cout << endl;
			}
		}
	}
	sqlite3_free_table(results);
}

int Database::executeQuery(std::string query) {

	int status = sqlite3_exec(getConnection(), query.c_str(), callback, 0, &this->_errorMessage);
    
	if (status != SQLITE_OK) {
		fprintf(stderr, "SQL Error: %s\n", this->_errorMessage);
		sqlite3_free(this->_errorMessage);
	}

	return status;
}

int Database::callback(void *notUsed, int argc, char **argv, char **columnName) {

	for (int i = 0; i < argc; ++i) {
		printf("%s = %s\n", columnName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
