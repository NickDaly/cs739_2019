#ifndef SERVER_H
#define SERVER_H

#include "debug.h"

class server {

public:
	server(int port);
	virtual ~server();
	void serve();
	void shutdown();
private:
	int port_;
};

#endif