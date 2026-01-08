#include "networking.h"

// 149.89.40.[100-134] (marge)

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
	fd_set read_fds;
	FD_ZERO(&read_fds);
	int fd_max = 0;
	int socks[nodes];
	while (listen_socket) {
		int client_socket = server_tcp_handshake(listen_socket);
		socks[nodes_conn] = client_socket;
		FD_SET(client_socket, &read_fds);
		if (fd_max < client_socket) fd_max = client_socket;
		nodes_conn++;
		if (nodes_conn == nodes) break;
	}
	printf("All nodes connected.\n");
	for (int i = 0; i<nodes; i++) {
		char task[128];
		snprintf(task, sizeof(task), "Node %d echo", i);
		write(socks[i], task, strlen(task));
	}
	err(select(fd_max+1, &read_fds, 0, 0, 0), "select error");
	for (int i = 0; i<nodes; i++) {
		char buff[128];
		if (!FD_ISSET(socks[i], &read_fds)) {
			FD_ZERO(&read_fds);
			err(select(fd_max+1, &read_fds, 0, 0, 0), "select error");
			i--;
		}
		else {
			buff[read(socks[i], buff, sizeof(buff))] = 0;
			printf("%s\n", buff);
		}
	}
	return 0;
}
