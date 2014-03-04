#include "database.h"

Database::Database(const std::string& path)
:_database(nullptr)
{
	const int result = sqlite3_open(path.c_str(), &_database);
	if (result != SQLITE_OK) {
		throw std::exception(sqlite3_errstr(result));
	}
}

Database::~Database()
{
	sqlite3_close(_database);
}