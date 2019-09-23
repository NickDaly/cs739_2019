#include "client.h"

client::client(const std::string &host, int port) {
	host_ = host;
	port_ = port;
	sock_ = 0;
}

client::~client() {
	if (sock_) {
		close(sock_);
		sock_ = 0;
	}
}


void client::send_message(const message &msg, message &response) {
	
	DEBUG_PRINT("client::send_message() [begin]");

	struct sockaddr_in server_address;

	memset(&server_address, 0, sizeof(server_address));
	
	server_address.sin_family = AF_INET;

	inet_pton(AF_INET, host_.c_str(), &server_address.sin_addr);

	server_address.sin_port = htons(port_);

	if ((sock_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        throw exception("client::send_message() Error opening socket", errno);
	}

	if (connect(sock_, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
		throw exception("client::send_message() Error connecting to end point", errno);
	}

	DEBUG_PRINT("client::send_message() send...");
	send(sock_, (void *) &msg, sizeof(message), 0);

	// get a response from the server
	int len = sizeof(message);
	int n = recv(sock_, (void *) &response, len, MSG_WAITALL);
	if (n<len) {
		throw exception("client::send_message() Invalid response size", errno);
	}

	close(sock_);
	sock_ = 0;

	DEBUG_PRINT("client::send_message() [end]");
}


bool client::get(std::string &key, std::vector<char> &value, int64_t &timestamp) {
	return false;
}


bool client::put(std::string &key, std::vector<char> &newvalue, std::vector<char> &oldvalue, int64_t &timestamp) {
	return false;
}
