#include <iostream>
#include "debug.h"
#include "lib739kv.h"
#include "sqlite3.h"


int main() {
	DEBUG_PRINT("main() [begin]");
	
	kv739_init(0);
	kv739_get(0, 0);
	kv739_put(0, 0, 0);
	kv739_shutdown();
	
	printf("SQLite3 Version:%s\n", sqlite3_libversion()); 

	DEBUG_PRINT("main() [end]");
	return 0;
}