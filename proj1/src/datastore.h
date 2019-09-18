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

	void get(std::string &key, std::vector<char> &value, int64_t &timestamp);

	void get(std::string &key, std::vector<char> &value) {
		int64_t ts;
		get(key, value, ts);
	}


	void set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp);

	void set(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue) {
		int64_t ts;
		set(key, newvalue, oldvalue, ts);
	}

	void set(std::string &key, std::vector<char> &newvalue) {
		std::vector<char> oldvalue;
		set(key, newvalue, oldvalue);		
	}

private:
	std::string filename_;
	sqlite3 *db_ = nullptr;
};
#endif