#include "networking.h"

// 149.89.40.[100-134] (marge)

int main() {
	signal(SIGCHLD, SIG_IGN); // prevents zombies
	int nodes;
	printf("How many nodes should be created? (1-35): ");
	scanf("%d", &nodes);
	if (nodes<1 || nodes>35) {
		printf("You must make between 1 and 35 nodes!\n");
		return 1;
	}
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
	char cmd[2048];
	char wd_path[1024];
	getcwd(wd_path, sizeof(wd_path));
	snprintf(cmd, sizeof(cmd), "%s/progn %s", wd_path, ip);
	char cmd2[2048];
	for (int i = 0; i<nodes; i++) {
		snprintf(cmd2, sizeof(cmd2), "sisakov60@149.89.40.%d", 100+i);
		char* args[] = {"ssh", cmd2, cmd, 0};
		if (!fork()) err(execvp(args[0], args), "execvp fail");
	}
	int nodes_conn = 0;
	int listen_socket = server_setup();
	fd_set read_fds;
	fd_set select_fds;
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
	select_fds = read_fds;
	err(select(fd_max+1, &select_fds, 0, 0, 0), "select error");
	for (int i = 0; i<nodes; i++) {
		char buff[128];
		if (!FD_ISSET(socks[i], &select_fds)) {
			FD_ZERO(&select_fds);
			select_fds = read_fds;
			err(select(fd_max+1, &select_fds, 0, 0, 0), "select error");
			i--;
		}
		else {
			buff[read(socks[i], buff, sizeof(buff))] = 0;
			printf("%s\n", buff);
		}
	}
	return 0;
}
