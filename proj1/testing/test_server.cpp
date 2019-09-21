#include "server.h"

int main() {
	std::string host = "localhost";
	server s(host, 52123);
	s.serve();
}