#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <memory>
#include <vector>
#include "debug.h"
#include "exception.h"
#include "sqlite3.h"
#include "sqlstatement.h"


using value_p = std::shared_ptr<std::vector<char>>;

class data_store {
public:
	data_store(std::string &filename);
	virtual ~data_store();

	bool get(std::string &key, std::vector<char> &value, int64_t &timestamp);

	bool get(std::string &key, std::vector<char> &value) {
		int64_t ts;
		return get(key, value, ts);
	}

	bool set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp);

	bool set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue) {
		int64_t ts;
		return set(key, newvalue, oldvalue, ts);
	}

	bool set(std::string &key, std::vector<char> &newvalue) {
		std::vector<char> oldvalue;
		return set(key, newvalue, oldvalue);		
	}

private:
	std::string filename_;
	sqlite3 *db_ = nullptr;
};
#endif