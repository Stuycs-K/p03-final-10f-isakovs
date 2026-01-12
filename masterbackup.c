#include "networking.h"

#define BYTES (1024LL*1024LL*1024LL*8LL)

int main() {
	signal(SIGCHLD, SIG_IGN); // prevents zombies
	char wd_path[1024];
	getcwd(wd_path, sizeof(wd_path));
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
	char cmd[4096];
	char cmd2[2048];
	long long seg = BYTES/((long long)nodes);
	for (int i = 0; i<nodes; i++) {
		snprintf(cmd, sizeof(cmd), "%s/progn %s %s %lld %lld", wd_path, ip, wd_path, ((long long)i)*seg, (i==nodes-1)?(BYTES):((long long)(i+1))*seg);
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
	int begin = clock();
	while (listen_socket) {
		int client_socket = server_tcp_handshake(listen_socket);
		socks[nodes_conn] = client_socket;
		FD_SET(client_socket, &read_fds);
		if (fd_max < client_socket) fd_max = client_socket;
		nodes_conn++;
		if (nodes_conn == nodes) break;
	}
	printf("All nodes connected.\n");
	/*
	for (int i = 0; i<nodes; i++) {
		char task[128];
		snprintf(task, sizeof(task), "Node %d echo", i);
		write(socks[i], task, strlen(task));
	}
	*/
	int max = INT_MIN;
	int nodes_finished = 0;
	struct timeval timeout;
	timeout.tv_sec = 5;  
	timeout.tv_usec = 0;
	while (nodes_finished < nodes) {
		select_fds = read_fds;
		int selret = select(fd_max + 1, &select_fds, NULL, NULL, NULL);
		if (!selret) {
			printf("sel returned zero\n");
			return 1;
		}
		for (int i = 0; i < nodes; i++) {
			if (FD_ISSET(socks[i], &select_fds)) {
				char buff[128];
				int n = read(socks[i], buff, sizeof(buff) - 1);
				if (n > 0) {
					buff[n] = '\0';
				    int curr = atoi(buff);
					printf("NEW NUMBER RECIEVED: %d\n", curr);
				    if (curr > max) max = curr;
				    close(socks[i]);
				    FD_CLR(socks[i], &read_fds);
				    nodes_finished++;
			    } else if (n == 0) {
		            close(socks[i]);
	                FD_CLR(socks[i], &read_fds);
					nodes_finished++;
				}
			}
		}
	}
	printf("Max is %d, took %f seconds.", max, (double)(clock()-begin)/CLOCKS_PER_SEC);
	return 0;
}
