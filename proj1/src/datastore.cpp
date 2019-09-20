#include <ctime>
#include <ratio>
#include <chrono>
#include <cstdint>
#include <cctype>

#include "datastore.h"
#include "sqlstatement.h"

int64_t get_timestamp() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}


data_store::data_store(std::string &filename) {
	DEBUG_PRINT("data_store::data_store() [begin]");
	//file:/home/fred/data.db
	DEBUG_PRINT("  Creating db connection to '%s'...", filename.c_str());
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
	DEBUG_PRINT("  done");
	DEBUG_PRINT("data_store::data_store() [end]");
}


data_store::~data_store() {
	DEBUG_PRINT("data_store::~data_store() [begin]");
	if (db_) {
		DEBUG_PRINT("  closing db connection...");
		sqlite3_close(db_);
		db_ = nullptr;
		DEBUG_PRINT("  done");
	}
	DEBUG_PRINT("data_store::~data_store() [end]");
}


bool data_store::get(std::string &key, std::vector<char> &value, int64_t &timestamp) {
	
	if (!validate_key(key)) {
		throw exception("sdata_store::get(): Invalid key", -1);
	}

	sql_statement stmt(db_);
	std::string sql = "SELECT key, value, timestamp from data_store WHERE key = ?";

	stmt.prepare(sql);
	stmt.bind_text(1, key);

	if (stmt.read()) {		
		
		auto k = stmt.read_text(0);

		//get the blob length
		int len = stmt.read_blob(1, 0, 0);
		//make sure the vector is in the same length
		value.resize(len);
		//read the value
		len = stmt.read_blob(1, value.data(), len);

		//return the timestamp		
		timestamp = stmt.read_int64(2);
		
		return true;
	}
	else {
		return false;
	}
}


bool data_store::put(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp) {	

	if (!validate_key(key)) {
		throw exception("sdata_store::put(): Invalid key", -1);
	}

	if (!validate_value(newvalue)) {
		throw exception("sdata_store::put(): Invalid value", -1);
	}

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


bool data_store::validate_key(const std::string &key) {

	if (key.length()==0) return false;
	
	if (key.length()>MAX_KEY_LEN) {
		return false;
	}

	for (auto c:key) {
		if (!isprint(c)) return false;		
		if (c=='[' || c==']') return false;			
	}
	
	return true;
}


bool data_store::validate_value(const std::vector<char> &data) {

	if (data.size()>MAX_KEY_LEN) {
		return false;
	}

	for (auto c:data) {
		if (!isprint(c)) return false;
		if (c=='[' || c==']') return false;	
	}

	return true;
}