#include "networking.h"

void clientLogic(int server_socket){
    char buff[BUFFER_SIZE];
    while (1) {
		/*
		memset(buff, 0, BUFFER_SIZE);
		printf("Enter line: ");
        if (!fgets(buff, BUFFER_SIZE, stdin)) {
			printf("Disconnected from server.\n");
			exit(0);
		}
		buff[strlen(buff)] = 0;
        write(server_socket, buff, 1+strlen(buff));
		*/
		memset(buff, 0, BUFFER_SIZE);
        if (!read(server_socket, buff, BUFFER_SIZE-1)) {
			//printf("Disconnected from server.\n");
			exit(0);
		}
		buff[strlen(buff)] = 0;
		write(server_socket, buff, strlen(buff));
		//char *temp = strchr(buff, '\n');
		//if (temp) *temp = 0;
        //printf("Received: %s\n", buff);
    }
}

int main(int argc, char *argv[] ) {
	//sleep(1);
	char* IP;
	if (argv[1]) IP = argv[1];
	else IP = "127.0.0.1";
	//printf("Using IP: %s\n", IP);
    int server_socket = client_tcp_handshake(IP);
    //printf("Connected to server.\n");
    clientLogic(server_socket);
    return 0;
}
