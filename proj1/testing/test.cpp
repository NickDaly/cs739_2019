#include <iostream>
#include <memory>
#include <vector>
#include "lib739kv.h"
#include "datastore.h"
#include "message.h"


std::string vec2str(std::vector<char> v) {
	std::string s(v.begin(), v.end());
	return s;
}


std::vector<char> str2vec(std::string &s) {
	std::vector<char> v(s.begin(), s.end());
	return v;
}


std::vector<char> str2vec(const char *s) {
	std::vector<char> v(s, s + strlen(s));
	return v;
}


void test_kv_store() {
	DEBUG_PRINT("  SQLite3 Version: %s\n", sqlite3_libversion()); 
	
	std::string file = "test.db";
	
	data_store ds(file);
	
	try {		
		std::vector<char> ov;
		ds.put("foo", "Hello World!");	
		ds.put("foo", "Goodbye World!", ov);
		std::cout << "old value: " << vec2str(ov) << std::endl;
	}
	catch (exception &ex) {
		std::cerr << ex.what()  << " error_code: " << ex.code() << std::endl;
	}


	try {
		int64_t ts;		
		std::string key = "foo";
		std::vector<char> value;		
		ds.get(key, value, ts);				
		std::string str(value.begin(), value.end());
		std::cout << "key  : " << key << std::endl;
		std::cout << "value: " << str << std::endl;
		std::cout << "time : " << ts << std::endl;		
	}
	catch (exception &ex) {
		std::cerr << ex.what()  << " error_code: " << ex.code() << std::endl;
	}

	try {
		int64_t ts;		
		std::string key = "foo";
		std::vector<char> value;		
		if (ds.get(key, value, ts)) {	
			std::string str(value.begin(), value.end());
			std::cout << "key  : " << key << std::endl;
			std::cout << "value: " << str << std::endl;
			std::cout << "time : " << ts << std::endl;		
		}
		else {
			std::cout << "'" << key  << "'" << " doesn't exist!" << std::endl;
		}
	}
	catch (exception &ex) {
		std::cerr << ex.what()  << " error_code: " << ex.code() << std::endl;
	}
}


void test_message() {
	DEBUG_PRINT("test_message() [begin]");
	message msg(command::OK);
	char v[] = {'a', 'b', 'c', 'd', 'e', 'f'};
	
	msg.set_key("foo");
	
	msg.set_value(v, sizeof(v));
	
	std::cout << "key=" << msg.key() << std::endl;

	std::cout << "value len=" << msg.get_value_size() << std::endl;

	for (int i=0; i<msg.get_value_size(); i++) {
		auto *val = msg.value();
		std::cout << val[i] << " ";
	}
	std::cout << std::endl;

	DEBUG_PRINT("test_message() [end]");
}


int main() {
	DEBUG_PRINT("main() [begin]");
	
	kv739_init(0);
	kv739_get(0, 0);
	kv739_put(0, 0, 0);	
	kv739_shutdown();
	
	test_kv_store();
	test_message();

	DEBUG_PRINT("main() [end]");
	return 0;
}