#include <sys/types.h> 
#include <sys/uio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "server.h"
#include "message.h"
#include "exception.h"

server::server(const std::string &host, int port) {
	DEBUG_PRINT("server::server() [begin]");
	DEBUG_PRINT("server::server() host=%s, port=%d", host.c_str(), port);
	port_ = port;
	host_ = host;
	DEBUG_PRINT("server::server() [end]");
}


server::~server() {
	DEBUG_PRINT("server::~server() [begin]");

	DEBUG_PRINT("server::~server() [end]");
}


void server::serve() {
    struct sockaddr_in serv_addr; 
 
 	sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_ < 0) { 
        throw exception("server::serve() Error opening socket", errno);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_);

    if (bind(sockfd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr))!=0 ) {
    	throw exception("server::serve() Error binding socket to port", errno);
	}
    
    if (listen(sockfd_, 5)!=0) {
    	throw exception("server::serve() Error binding socket to port", errno);
    }

	running_ = true;
 	listening_thread_ = new std::thread(&server::connection_handler, this);
 	processing_thread_ = new std::thread(&server::message_handler, this);

}


void server::connection_handler() {
	int newsockfd;
	struct sockaddr_in cli_addr;
    while (running_) {
    	socklen_t clilen = sizeof(cli_addr);
    	newsockfd = accept(sockfd_, (struct sockaddr *) &cli_addr, &clilen);
    	if (newsockfd < 0) {
    		throw exception("server::serve() Error accepting connection", errno);
    	}
    	conns_.enqueue(newsockfd);
	}
}


void server::message_handler() {

	while (running_) {

		int sockfd;

		if (conns_.dequeue_wait(sockfd)) {

			message msg;
			int n = read(sockfd, (char *) &msg, sizeof(msg));
			if (n < 0) {
				DEBUG_PRINT("server::message_handler() Error reading message");
			}

			switch (msg.get_command()) {
				case command::NONE:

					break;
					
				case command::CHK:

					break;

				case command::OK:

					break;

    			case command::GET:

					break;

    			case command::PUT:

					break;

    			case command::ERROR:

					break;

    			case command::SHUT_DOWN:
    				shutdown();
					break;

				default:
					break;
			}
		}

	}
}


void server::shutdown() {
	running_ = false;
	conns_.clear();
}


// #include <iostream> 
// #include <sys/ipc.h> 
// #include <sys/shm.h> 
// #include <stdio.h> 
// using namespace std; 

// int main() 
// { 
// 	// ftok to generate unique key 
// 	key_t key = ftok("shmfile",65); 

// 	// shmget returns an identifier in shmid 
// 	int shmid = shmget(key,1024,0666|IPC_CREAT); 

// 	// shmat to attach to shared memory 
// 	char *str = (char*) shmat(shmid,(void*)0,0); 

// 	cout<<"Write Data : "; 
// 	gets(str); 

// 	printf("Data written in memory: %s\n",str); 
	
// 	//detach from shared memory 
// 	shmdt(str); 

// 	return 0; 
// } 



// #include <iostream> 
// #include <sys/ipc.h> 
// #include <sys/shm.h> 
// #include <stdio.h> 
// using namespace std; 
  
// int main() 
// { 
//     // ftok to generate unique key 
//     key_t key = ftok("shmfile",65); 
  
//     // shmget returns an identifier in shmid 
//     int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
//     // shmat to attach to shared memory 
//     char *str = (char*) shmat(shmid,(void*)0,0); 
  
//     printf("Data read from memory: %s\n",str); 
      
//     //detach from shared memory  
//     shmdt(str); 
    
//     // destroy the shared memory 
//     shmctl(shmid,IPC_RMID,NULL); 
     
//     return 0; 
// } 
