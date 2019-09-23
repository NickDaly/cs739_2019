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


data_store::data_store() {

}


data_store::data_store(const char *filename) {
	DEBUG_PRINT("data_store::data_store() [begin]");
	//file:/home/fred/data.db
	DEBUG_PRINT("  Creating db connection to '%s'...", filename);

	filename_ = std::string(filename);
	
	auto ret = sqlite3_open(filename, &db_);
	
	if (ret!=SQLITE_OK) {
		throw exception("Unable to open database: " + filename_, ret);
	}

	const char *sql = "CREATE TABLE IF NOT EXISTS data_store (key TEXT PRIMARY KEY, value BLOB, timestamp INTEGER);";
 	
 	ret = sqlite3_exec(db_, sql, 0, 0, 0);
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


bool data_store::get(const char *key, const char *value, int *len, int64_t *timestamp) {

	if (!validate_key(key)) {
		throw exception("sdata_store::get(): Invalid key", -1);
	}

	if (timestamp) { 
		*timestamp = 0;
	}

	sql_statement stmt(db_);
	const char* sql = "SELECT key, value, timestamp from data_store WHERE key = ?";

	stmt.prepare(sql);

	stmt.bind_text(1, key);

	if (stmt.read()) {

	 	auto k = stmt.read_text(0);
	
		//get the blob length
		int vlen = stmt.read_blob(1, 0, 0);
		if (vlen > *len) {
			throw exception("data_store::get(): Insufficient buffer size", -1);	
		}

		*len = stmt.read_blob(1, value, vlen);

		//return the timestamp		
		if (timestamp) {
			*timestamp = stmt.read_int64(2);
		}
		
		return true;
	}
	else {
		return false;
	}
}


bool data_store::put(const char *key, const char *value, int len, const char *ov, int *ov_len, int64_t *timestamp) {	

	// if (!validate_key(key)) {
	// 	throw exception("sdata_store::put(): Invalid key", -1);
	// }

	// if (!validate_value(value, len)) {
	// 	throw exception("sdata_store::put(): Invalid value", -1);
	// }

	// timestamp = 0;

	// sql_statement stmt(db_);

	// auto ts = get_timestamp();
	
	// if (get(key, oldvalue, timestamp)) {
	// 	std::string sql = "UPDATE data_store SET value = ?, timestamp = ? WHERE key = ?";
	// 	stmt.prepare(sql);
	// 	stmt.bind_blob(1, newvalue.data(), newvalue.size());
	// 	stmt.bind_int64(2, ts);
	// 	stmt.bind_text(3, key);
	// 	stmt.execute();
	// }
	// else {
	// 	std::string sql = "INSERT INTO data_store VALUES(?, ?, ?)";
	// 	stmt.prepare(sql);
	// 	stmt.bind_text(1, key);
	// 	stmt.bind_blob(2, newvalue.data(), newvalue.size());
	// 	stmt.bind_int64(3, ts);
	// 	stmt.execute();
	// }

	return true;
}


bool data_store::validate_key(const char *key) {
	int len = strlen(key);
	if (len==0 || len>MAX_KEY_LEN) return false;	
	for (int i=0; i<len; i++) {
		auto c = key[i];
		if (!isprint(c)) return false;		
		if (c=='[' || c==']') return false;			
	}	
	return true;
}


bool data_store::validate_value(const char *data, int len) {

	if (len>MAX_KEY_LEN) return false;
	
	for (int i=0; i<len; i++) {
		auto c = data[i];
		if (!isprint(c)) return false;
		if (c=='[' || c==']') return false;	
	}

	return true;
}