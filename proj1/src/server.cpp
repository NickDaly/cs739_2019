#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
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

 	int sockfd;
 	int newsockfd;

    struct sockaddr_in serv_addr; 
    struct sockaddr_in cli_addr;
 	
 	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { 
        throw exception("server::serve() Error opening socket", errno);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))!=0 ) {
    	throw exception("server::serve() Error binding socket to port", errno);
	}
    
    if (listen(sockfd, 5)!=0) {
    	throw exception("server::serve() Error binding socket to port", errno);
    }

    socklen_t clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
    	throw exception("server::serve() Error accepting connection", errno);
    }

     //      error("ERROR on accept");
     // bzero(buffer,256);
     // n = read(newsockfd,buffer,255);
     // if (n < 0) error("ERROR reading from socket");
     // printf("Here is the message: %s\n",buffer);
     // n = write(newsockfd,"I got your message",18);
     // if (n < 0) error("ERROR writing to socket");
     // return 0; 
}


void server::shutdown() {


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
