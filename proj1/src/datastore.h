#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <memory>
#include <vector>
#include "debug.h"
#include "exception.h"
#include "sqlite3.h"
#include "sqlstatement.h"


#define MAX_KEY_LEN 128
#define MAX_VALUE_LEN 2048
#define INVALID_CHARS "[]"

using value_p = std::shared_ptr<std::vector<char>>;

class data_store {
public:
	data_store(std::string &filename);
	virtual ~data_store();

	virtual bool get(std::string &key, std::vector<char> &value, int64_t &timestamp);
	virtual bool set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp);
	virtual bool validate_key(const std::string &key);
	virtual bool validate_value(const std::vector<char> &data);

	bool get(const char *key, std::vector<char> &value, int64_t &timestamp) {
		std::string s(key);
		return get(s, value, timestamp);
	}

	bool get(std::string &key, std::vector<char> &value) {
		int64_t ts;
		return get(key, value, ts);
	}	
	
	bool set(const char *key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp) {
		std::string s(key);
		return set(s, newvalue, oldvalue, timestamp);
	}

	bool set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue) {
		int64_t ts;
		return set(key, newvalue, oldvalue, ts);
	}

	bool set(std::string &key, std::vector<char> &newvalue) {
		std::vector<char> oldvalue;
		return set(key, newvalue, oldvalue);		
	}
	
	bool set(const char *key, std::vector<char> &newvalue) {
		std::string s(key);
		return set(s, newvalue);
	}

	bool set(const char *key, const char *value) {
		std::vector<char> data(value, value + strlen(value));
		std::string s(key);
		return set(s, data);
	}

	bool set(const char *key, const char *value, std::vector<char> &oldvalue) {
		std::vector<char> data(value, value + strlen(value));
		std::string s(key);
		return set(s, data, oldvalue);
	}

private:
	std::string filename_;
	sqlite3 *db_ = nullptr;
};
#endif