#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <memory>
#include <vector>
#include "debug.h"
#include "sqlite3.h"
#include "exception.h"

using value_p = std::shared_ptr<std::vector<char>>;

class data_store {
public:
	data_store(std::string &filename);
	virtual ~data_store();
	void get(std::string &key, value_p &value);
	void set(std::string &key, std::vector<char> &newvalue, value_p &oldvalue);
private:
	std::string filename_;
	sqlite3 *pDb_ = nullptr;
};
#endif