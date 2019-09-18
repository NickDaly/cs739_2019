#include <iostream>
#include <memory>
#include <vector>
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

	std::vector<char> newvalue;
	std::vector<char> oldvalue;
	
	std::string key = "foo";
	
	try {
		ds.set(key, value, oldvalue);
	}
	catch (exception &ex) {
		std::cerr << ex.what()  << " error_code: " << ex.code() << std::endl;
	}

	try {
		int64_t t;

		ds.get(key, newvalue, t);

		std::cout << "key:" << key << std::endl;

		std::cout << "value:";
		for (auto v:newvalue) {
			std::cout << v ;
		}
		std::cout << std::endl;
		std::cout << "time:" << t << std::endl;		
	}
	catch (exception &ex) {
		std::cerr << ex.what()  << " error_code: " << ex.code() << std::endl;
	}


	DEBUG_PRINT("main() [end]");
	return 0;
}