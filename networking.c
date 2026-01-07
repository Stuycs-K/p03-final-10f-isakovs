#include "networking.h"

/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
	//setup structs for getaddrinfo
	int status;
	struct addrinfo *hints;
	struct addrinfo *res;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	hints->ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(NULL, PORT, hints, &res)) < 0) {
		printf("Getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}
	
	//create the socket
	int clientd;//store the socket descriptor here
	clientd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	err(clientd, "socket");
	
	//this code should get around the address in use error
	int yes = 1;
	int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	err(sockOpt,"sockopt  error");
	
	//bind the socket to address and port
	if (bind(clientd, res->ai_addr, res->ai_addrlen) == -1) {
		close(clientd);
		err(-1, "bind");
	}

	//set socket to listen state
	err(listen(clientd, BACKLOG), "listen");

	//free the structs used by getaddrinfo
	free(hints);
	freeaddrinfo(res);
	return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;
    //accept() the client connection
	struct sockaddr_in client_address;
	socklen_t sock_size = sizeof(client_address);
	client_socket = accept(listen_socket, (struct sockaddr*)&client_address, &sock_size);
    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
	//getaddrinfo
	int status;
	struct addrinfo *hints;
	struct addrinfo *res;
	hints = calloc(1, sizeof(struct addrinfo));
	hints->ai_family = AF_INET;
	hints->ai_socktype = SOCK_STREAM;
	//hints->ai_flags = AI_PASSIVE;
	if ((status = getaddrinfo(server_address, PORT, hints, &res)) < 0) {
		printf("Getaddrinfo error: %s\n", gai_strerror(status));
		exit(1);
	}

	int serverd;//store the socket descriptor here
	//create the socket
	err((serverd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)), "socket");

	//connect() to the server
	err(connect(serverd, res->ai_addr, res->ai_addrlen), "connect");

	free(hints);
	freeaddrinfo(res);

	return serverd;
}




void err(int i, char*message){
	if(i < 0){
		printf("Error: %s - %s\n",message, strerror(errno));
		exit(1);
	}
}
