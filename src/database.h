#pragma once

#include <string>
#include <sqlite3/sqlite3.h>

class Database
{
public:
	Database(const std::string& path);
	~Database();

private:
	sqlite3* _database;
};