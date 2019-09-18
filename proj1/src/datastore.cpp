#include "datastore.h"


data_store::data_store(std::string &filename) {
	DEBUG_PRINT("data_store::data_store() [begin]");
	//file:/home/fred/data.db
	filename_ = filename;
	auto ret = sqlite3_open(filename.c_str(), &pDb_);
	
	if (ret!=SQLITE_OK) {
		throw exception("Unable to open database: " + filename, ret);
	}

	std::string sql = "CREATE TABLE IF NOT EXISTS data_store (key TEXT PRIMARY KEY, value BLOB, timestamp INTEGER);";
 	
 	ret = sqlite3_exec(pDb_, sql.c_str(), 0, 0, 0);
 	if (ret!=SQLITE_OK) {
 		throw exception("Error creating the date_store table", ret);
 	}
	
	DEBUG_PRINT("data_store::data_store() [end]");
}


data_store::~data_store() {
	DEBUG_PRINT("data_store::~data_store() [begin]");
	if (pDb_) {
		sqlite3_close(pDb_);
		pDb_ = nullptr;
	}
	DEBUG_PRINT("data_store::~data_store() [end]");
}


void data_store::get(std::string &key, value_p &value, int64_t &timestamp) {
	std::string sql = "SELECT key, value, timestamp from data_store WHERE key = ?";
	sqlite3_stmt *stmt;
	timestamp = 0;
	value = nullptr;

	auto ret = sqlite3_prepare_v2(pDb_, sql.c_str(), -1, &stmt, 0);
    if (ret != SQLITE_OK) {
    	throw exception("Faild to execute statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }
    
    ret = sqlite3_bind_text(stmt, 1, key.c_str(), -1, 0);
    if (ret != SQLITE_OK) {
    	throw exception("Faild to bind key variable:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

    ret = sqlite3_step(stmt);
    
    if (ret == SQLITE_ROW) {
		int length = sqlite3_column_bytes(stmt, 1);
		auto v = std::make_shared<std::vector<char>> (length);
		auto *pbuf = reinterpret_cast<const char*>( sqlite3_column_blob(stmt, 1));
		std::copy(pbuf, pbuf + v->size(), v->data());
    	value = v;
  	   	timestamp = sqlite3_column_int64(stmt, 2);
    }
    
    sqlite3_finalize(stmt);
   
}


void data_store::set(std::string &key, std::vector<char> &newvalue, value_p &oldvalue) {
	std::string sql = "INSERT INTO data_store VALUES(?, ?, ?)";

	value_p oldvalue;
	int64_t ts;

	try {
		get(key, oldvalue, ts);
		
		std::string sql ="UPDATE data_store SET value = ?, timestamp = ? WHERE key = ?"
		

	}
	catch (execute &ex) {

	}

	oldvalue = nullptr;
	
	sqlite3_stmt *stmt;
	
	auto ret = sqlite3_prepare_v2(pDb_, sql.c_str(), -1, &stmt, 0);
	if (ret != SQLITE_OK) {
    	throw exception("Faild to prepare statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

	ret = sqlite3_bind_text(stmt, 1, key.c_str(), -1, 0);
	if (ret != SQLITE_OK) {
    	throw exception("Faild to bind text:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

	ret = sqlite3_bind_blob(stmt, 2, newvalue.data(), newvalue.size(), SQLITE_STATIC);
	if (ret != SQLITE_OK) {
    	throw exception("Faild to bind blob:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

	ret = sqlite3_bind_int64(stmt, 3, 100);
	if (ret != SQLITE_OK) {
    	throw exception("Faild to bind int64:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

    ret = sqlite3_step(stmt);
   	if (ret != SQLITE_DONE) {
    	throw exception("Faild to execute statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }

    ret = sqlite3_finalize(stmt);
  	if (ret != SQLITE_OK) {
    	throw exception("Faild to finalize insert statement:" + std::string(sqlite3_errmsg(pDb_)), ret);
    }
}