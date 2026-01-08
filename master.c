#include "networking.h"

// the number of nodes connected may stay scoped in main
// use forking to make new nodes. If a node is closed then close all socks and error.
// 149.89.40.[100-134] (marge)

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
	close(client_socket);
	exit(0);
}

int main() {
	signal(SIGCHLD, SIG_IGN); // prevents zombies
	int nodes;
	printf("How many nodes should be created? (1-30): ");
	scanf("%d", &nodes);
	FILE* fp;
	char buff1[64];
	char ip[64] = "";
	fp = popen("/bin/hostname -I", "r");
	if (!fp) {
		printf("popen fail\n");
		exit(1);
	}
	while (fgets(buff1, sizeof(buff1), fp)) strcat(ip, buff1);
	pclose(fp);
	ip[strlen(ip)-1] = 0;
	printf("Server IP: %s\n", ip);
	char cmd[128];
	snprintf(cmd, sizeof(cmd), "~/Documents/c/p03-final-10f-isakovs/progn %s", ip);
	char* args[] = {"ssh", "sisakov60@marge.stuy.edu", cmd, 0};
	for (int i = 0; i<nodes; i++) {
		if (!fork()) err(execvp(args[0], args), "execvp fail");
	}
	int nodes_conn = 0;
	int listen_socket = server_setup();
	while (listen_socket) {
		int client_socket = server_tcp_handshake(listen_socket);
		if (!fork()) subserver(client_socket);
		nodes_conn++;
		printf("Nodes connected: %d\n", nodes_conn);
		close(client_socket);
	}
	return 0;
}
