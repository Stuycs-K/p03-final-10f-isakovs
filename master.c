#include "networking.h"

// set shm for storing task keys
// the number of nodes connected may stay scoped in main
// the number of nodes increases at new socket connection
// the number of nodes decreases at disconnect signal
//

void subserver(int client_socket) {
	char buff[BUFFER_SIZE];
	memset(buff, 0, BUFFER_SIZE);
	int bytes;
	while ((bytes = read(client_socket, buff, BUFFER_SIZE)) > 0) {
		buff[bytes] = 0;
		char* out = "testing";
		write(client_socket, out, 1+strlen(out));
		memset(buff, 0, BUFFER_SIZE);
	}
	// send signal to main that the node has been disconnected
	close(client_socket);
	exit(0);
}

int main() {
	signal(SIGCHLD, SIG_IGN);
	int listen_socket = server_setup();
	while (listen_socket) {
		int client_socket = server_tcp_handshake(listen_socket);
		if (!fork()) subserver(client_socket);
		close(client_socket);
	}
	return 0;
}
