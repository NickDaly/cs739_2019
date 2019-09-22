#include <iostream>
#include <string>

#include "server.h"

int main() {
	std::string host = "localhost";
	server s(host, 52123);
	s.serve();

	while (1) {
		std::cout << "Enter 'q' to stop...";
		std::string ans;
		std::cin >> ans;
		if (ans=="q") break;
	}
}