#include <iostream>
#include <string>

#include "client.h"

int main() {
	std::string host = "localhost";
	client c(host, 52123);

	while (1) {
		
		std::string key;
		std::string value;

		std::cout << "Enter a key:";		
		std::cin >> key;
		
		std::cout << "Enter a value:";
		std::cin >> value;

		message msg(command::PUT);

		msg.set_key(key.c_str());
		msg.set_value(value.c_str(), value.length());

		message res;
		c.send_message(msg, res);
	}

	message msg(command::SHUT_DOWN);
	c.send_message(msg, msg);

}