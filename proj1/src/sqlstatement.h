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
	void bind_txt(const std::string &txt, int pos);
	void bind_blob(const char *blob, int length, int pos);
	void bind_int64(int64_t value, int pos);
	bool read();
	std::string read_text(int col);
	int read_blob(const char *buffer, int length);
	int64_t read_int64(int col);
private:
	sqlite3_stmt *stmt_ = nullptr;
	sqlite3 *db_ = nullptr;
};
#endif