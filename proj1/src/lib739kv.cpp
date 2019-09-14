#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "lib739kv.h"
#include "server.h"

int kv739_init(char ** server_list) {
	std::cout << "kv739_init" << std::endl;	
	return 0;
}

int kv739_shutdown(void) {
	std::cout << "kv739_shutdown" << std::endl;	
	return 0;
}

int kv739_get(char * key, char * value) {
	std::cout << "kv739_get" << std::endl;	
	return 0;
}

int kv739_put(char * key, char * value, char * old_value) {
	std::cout << "kv739_put" << std::endl;	
	return 0;
}
