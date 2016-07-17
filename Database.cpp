#include "stdafx.h"
#include "Database.h"
#include <ios>


Database::Database(std::string filename): _filename(filename)
{
	// initialize engine
	int status = 0;
	if (SQLITE_OK != (status = sqlite3_initialize()))
	{
		printf("Failed to initialize library: %d\n", status);
	}
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
		return false;
	}
	else {
		cout << "Opened " << filename.c_str() << " successfully" << std::endl;
	}
	return true;
}

void Database::displayTable(std::string tableName) {
	// Display MyTable
	std::cout << "Retrieving values in MyTable ..." << std::endl;
	const char *sqlSelect = "SELECT * FROM MyTable;";
	char **results = NULL;
	int rows, columns;
	char *error;

	int status = sqlite3_get_table(_connection, sqlSelect, &results, &rows, &columns, &error);
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
