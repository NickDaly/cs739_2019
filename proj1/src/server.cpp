#include "debug.h"
#include "server.h"


server::server(int port) {
	DEBUG_PRINT("server::server(%d)", port);
	port_ = port;
}

server:: ~server() {
	DEBUG_PRINT("server:: ~server()");
}

void server::serve() {

}

void server::shutdown() {

}