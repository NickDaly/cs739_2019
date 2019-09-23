#ifndef DATASTORE_H
#define DATASTORE_H

#include <string>
#include <memory>
#include <vector>
#include "sqlite3.h"
#include "debug.h"
#include "exception.h"
#include "sqlstatement.h"

#define MAX_KEY_LEN 128
#define MAX_VALUE_LEN 2048

int64_t get_timestamp();


class data_store {
public:
	data_store();
	data_store(const char *filename);
	virtual ~data_store();

	virtual bool get(const char *key, const char *value, int *len, int64_t *timestamp);
	virtual bool put(const char *key, const char *value, int len, const char *ov, int *ov_len, int64_t *timestamp);

	virtual bool validate_key(const char* key);
	virtual bool validate_value(const char *value, int len);

	virtual bool get(const char *key, const char *value, int *len) {
		int64_t ts;
		return get(key, value, len, &ts);
	}

	virtual bool put(const char *key, const char *value, int len) {
		char ov[MAX_VALUE_LEN];
		int ov_len = sizeof(ov);
		int64_t ts;
		return put(key, value, len, ov, &ov_len, &ts);
	}

	virtual bool put(const char *key, const char *value) {
		int len = strlen(value);
		return put(key, value, len);
	}

private:
	std::string filename_;
	sqlite3 *db_ = nullptr;
};
#endif