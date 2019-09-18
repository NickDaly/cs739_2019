#include "sqlstatement.h"

sql_statement::sql_statement(sqlite3 *db) {
	db_ = db;
}


sql_statement::~sql_statement() {
	if (stmt_) {
		sqlite3_finalize(stmt_);
		stmt_ = nullptr;
	}
}


void sql_statement::prepare(const std::string &sql) {
	auto ret = sqlite3_prepare_v2(db_, sql.c_str(), -1, &stmt_, 0);
	if (ret != SQLITE_OK) {
		throw exception("sql_statement::prepare(): Faild to prepare statement ':" + sql + "' Error:" + std::string(sqlite3_errmsg(db_)), ret);
	}
}


void sql_statement::execute(const std::string &sql) {	 	
	auto ret = sqlite3_exec(db_, sql.c_str(), 0, 0, 0);
	if (ret!=SQLITE_OK) {
		throw exception("sql_statement::execute(): Faild to execute statement ':" + sql + "' Error:" + std::string(sqlite3_errmsg(db_)), ret);
	}
}


void sql_statement::execute() {
	auto ret = sqlite3_step(stmt_);
	if (ret != SQLITE_DONE) {
		throw exception("sql_statement::execute():" + std::string(sqlite3_errmsg(db_)), ret);
	}
}


void sql_statement::bind_txt(const std::string &txt, int pos) {
	auto ret = sqlite3_bind_text(stmt_, pos, txt.c_str(), -1, 0);
	if (ret != SQLITE_OK) {
		throw exception("sql_statement::bind_txt(): " + std::string(sqlite3_errmsg(db_)), ret);
	}
}


void sql_statement::bind_blob(const char *blob, int length, int pos) {
	auto ret = sqlite3_bind_blob(stmt_, pos, blob, length, SQLITE_STATIC);
	if (ret != SQLITE_OK) {
		throw exception("sql_statement::bind_blob():" + std::string(sqlite3_errmsg(db_)), ret);
	}
}


void sql_statement::bind_int64(int64_t value, int pos) {
	auto ret = sqlite3_bind_int64(stmt_, pos, value);
	if (ret != SQLITE_OK) {
		throw exception("Faild to bind int64:" + std::string(sqlite3_errmsg(db_)), ret);
	}
}


bool sql_statement::read() {
	auto ret = sqlite3_step(stmt_);    
	if (ret == SQLITE_ROW) {
		return true;
	}
	return false;
}


std::string sql_statement::read_text(int col) {
	
	return "";
}


int sql_statement::read_blob(const char *buffer, int length) {
	
	return 0;
}


int64_t sql_statement::read_int64(int col) {
	
	return 0;
}