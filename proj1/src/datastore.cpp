#include <ctime>
#include <ratio>
#include <chrono>
#include <cstdint>

#include "datastore.h"
#include "sqlstatement.h"

int64_t get_timestamp() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

data_store::data_store(std::string &filename) {
	DEBUG_PRINT("data_store::data_store() [begin]");
	//file:/home/fred/data.db
	
	filename_ = filename;
	
	auto ret = sqlite3_open(filename.c_str(), &db_);
	
	if (ret!=SQLITE_OK) {
		throw exception("Unable to open database: " + filename, ret);
	}

	std::string sql = "CREATE TABLE IF NOT EXISTS data_store (key TEXT PRIMARY KEY, value BLOB, timestamp INTEGER);";
 	
 	ret = sqlite3_exec(db_, sql.c_str(), 0, 0, 0);
 	if (ret!=SQLITE_OK) {
 		throw exception("Error creating the date_store table", ret);
 	}
	
	DEBUG_PRINT("data_store::data_store() [end]");
}


data_store::~data_store() {
	DEBUG_PRINT("data_store::~data_store() [begin]");
	if (db_) {
		sqlite3_close(db_);
		db_ = nullptr;
	}
	DEBUG_PRINT("data_store::~data_store() [end]");
}


bool data_store::get(std::string &key, std::vector<char> &value, int64_t &timestamp) {
	std::cout << "get:" << get_timestamp() << std::endl;
	sql_statement stmt(db_);
	std::string sql = "SELECT value, timestamp from data_store WHERE key = ?";

	stmt.prepare(sql);
	stmt.bind_text(1, key);

	if (stmt.read()) {		
		//get the blob length
		int len = stmt.read_blob(0, 0, 0);
		//make sure the vector is in the same length
		value.resize(len);
		//read the value
		len = stmt.read_blob(0, value.data(), len);
		timestamp = stmt.read_int64(1);
		return true;
	}
	else {
		return false;
	}
}


bool data_store::set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp) {
	std::cout << "set:" << get_timestamp() << std::endl;
	sql_statement stmt(db_);

	auto ts = get_timestamp();
	if (get(key, oldvalue, timestamp)) {
		std::string sql = "UPDATE data_store SET value = ?, timestamp = ? WHERE key = ?";
		stmt.prepare(sql);
		stmt.bind_blob(1, newvalue.data(), newvalue.size());
		stmt.bind_int64(2, ts);
		stmt.bind_text(3, key);
		stmt.execute();
	}
	else {
		std::string sql = "INSERT INTO data_store VALUES(?, ?, ?)";
		stmt.prepare(sql);
		stmt.bind_text(1, key);
		stmt.bind_blob(2, newvalue.data(), newvalue.size());
		stmt.bind_int64(3, ts);
		stmt.execute();
	}
	return true;
}


// void data_store::get(std::string &key, std::vector<char>  &value, int64_t &timestamp) {
// 	// std::string sql = "SELECT key, value, timestamp from data_store WHERE key = ?";
// 	// sqlite3_stmt *stmt;
// 	// timestamp = 0;
// 	// value = nullptr;

// 	// auto ret = sqlite3_prepare_v2(pDb_, sql.c_str(), -1, &stmt, 0);
//  //    if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to execute statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }
    
//  //    ret = sqlite3_bind_text(stmt, 1, key.c_str(), -1, 0);
//  //    if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to bind key variable:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

//  //    ret = sqlite3_step(stmt);
    
//  //    if (ret == SQLITE_ROW) {
// 	// 	int length = sqlite3_column_bytes(stmt, 1);
// 	// 	auto v = std::make_shared<std::vector<char>> (length);
// 	// 	auto *pbuf = reinterpret_cast<const char*>( sqlite3_column_blob(stmt, 1));
// 	// 	std::copy(pbuf, pbuf + v->size(), v->data());
//  //    	value = v;
//  //  	   	timestamp = sqlite3_column_int64(stmt, 2);
//  //    }
    
//  //    sqlite3_finalize(stmt);
   
// }


// void data_store::set(std::string &key, std::vector<char> &newvalue, std::vector<char>  &oldvalue) {
// 	// std::string sql = "INSERT INTO data_store VALUES(?, ?, ?)";

// 	// value_p oldvalue;
// 	// int64_t ts;

// 	// try {
// 	// 	get(key, oldvalue, ts);
		
// 	// 	std::string sql ="UPDATE data_store SET value = ?, timestamp = ? WHERE key = ?"
		

// 	// }
// 	// catch (execute &ex) {

// 	// }

// 	// oldvalue = nullptr;
	
// 	// sqlite3_stmt *stmt;
	
// 	// auto ret = sqlite3_prepare_v2(pDb_, sql.c_str(), -1, &stmt, 0);
// 	// if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to prepare statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

// 	// ret = sqlite3_bind_text(stmt, 1, key.c_str(), -1, 0);
// 	// if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to bind text:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

// 	// ret = sqlite3_bind_blob(stmt, 2, newvalue.data(), newvalue.size(), SQLITE_STATIC);
// 	// if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to bind blob:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

// 	// ret = sqlite3_bind_int64(stmt, 3, 100);
// 	// if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to bind int64:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

//  //    ret = sqlite3_step(stmt);
//  //   	if (ret != SQLITE_DONE) {
//  //    	throw exception("Faild to execute statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }

//  //    ret = sqlite3_finalize(stmt);
//  //  	if (ret != SQLITE_OK) {
//  //    	throw exception("Faild to finalize insert statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
//  //    }
// }