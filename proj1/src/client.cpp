#include "client.h"

client::client(const std::string &host, int port) {
	host_ = host;
	port_ = port;
}

client::~client() {

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


// void test() {
// 	const char* server_name = "localhost";
// 	const int server_port = 8877;

// 	struct sockaddr_in server_address;
// 	memset(&server_address, 0, sizeof(server_address));
// 	server_address.sin_family = AF_INET;

// 	// creates binary representation of server name
// 	// and stores it as sin_addr
// 	// http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
// 	inet_pton(AF_INET, server_name, &server_address.sin_addr);

// 	// htons: port in network order format
// 	server_address.sin_port = htons(server_port);

// 	// open a stream socket
// 	int sock;
// 	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
// 		printf("could not create socket\n");

// 	}

// 	// TCP is connection oriented, a reliable connection
// 	// **must** be established before any data is exchanged
// 	if (connect(sock, (struct sockaddr*)&server_address,
// 	            sizeof(server_address)) < 0) {
// 		printf("could not connect to server\n");

// 	}

// 	// send

// 	// data that will be sent to the server
// 	const char* data_to_send = "Gangadhar Hi Shaktimaan hai";
// 	send(sock, data_to_send, strlen(data_to_send), 0);

// 	// receive

// 	int n = 0;
// 	int len = 0, maxlen = 100;
// 	char buffer[maxlen];
// 	char* pbuffer = buffer;

// 	// will remain open until the server terminates the connection
// 	while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
// 		pbuffer += n;
// 		maxlen -= n;
// 		len += n;

// 		buffer[len] = '\0';
// 		printf("received: '%s'\n", buffer);
// 	}

// 	// close the socket
// 	close(sock);
// }