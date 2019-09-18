#include <iostream>
#include <memory>
#include <vector>
#include "debug.h"
#include "lib739kv.h"
#include "datastore.h"


int main() {
	DEBUG_PRINT("main() [begin]");
	
	kv739_init(0);
	kv739_get(0, 0);
	kv739_put(0, 0, 0);
	kv739_shutdown();
	
	printf("SQLite3 Version:%s\n", sqlite3_libversion()); 
	
	std::string file = "test.db";
	data_store ds(file);
	
	std::vector<char> value;
	value.push_back('a');
	value.push_back('b');
	value.push_back('c');

	value_p newvalue;
	value_p oldvalue;
	
	std::string key = "foo";

	ds.set(key, value, oldvalue);

	DEBUG_PRINT("main() [end]");
	return 0;
}