#ifndef SQLCOMMAND_H
#define SQLCOMMAND_H

#include <string>
#include "sqlite3.h"
#include "exception.h"

class sql_statement {
public:
	sql_statement(sqlite3 *db);	
	virtual ~sql_statement();
	void prepare(const std::string &stmt);
	void execute(const std::string &stmt);
	void execute();
	void bind_text(int pos, const std::string &txt);
	void bind_blob(int pos, const char *buffer, int length);
	void bind_int64(int pos, int64_t value);
	bool read();
	std::string read_text(int col);
	int read_blob(int col, const char *buffer, int length);
	int64_t read_int64(int col);
private:
	sqlite3_stmt *stmt_ = nullptr;
	sqlite3 *db_ = nullptr;
};
#endif