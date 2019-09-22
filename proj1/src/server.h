#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "queue.h"
#include "debug.h"

class server {

public:
	server(const std::string &host, int port);
	virtual ~server();
	void serve();
	void connection_handler();
	void message_handler();
	void shutdown();
	
private:
	int port_;
	std::string host_;
	int sockfd_;
	queue<int> conns_;
	bool running_;
	// bool waiting_;
	std::thread* listening_thread_ = nullptr;
	std::thread* processing_thread_ = nullptr;
};

#endif