#include <iostream>
#include "lib739kv.h"

int main() {
	std::cout << "test [begin]" << std::endl;
	kv739_init(0);
	kv739_shutdown();
	kv739_get(0, 0);
	kv739_put(0, 0, 0);
	std::cout << "test [end]" << std::endl;
	return 0;
}