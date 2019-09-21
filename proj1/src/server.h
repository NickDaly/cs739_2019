#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "debug.h"

class server {

public:
	server(const std::string &host, int port);
	virtual ~server();
	void serve();
	void shutdown();
private:
	int port_;
	std::string host_;
};

#endif