#include "networking.h"

#define BYTES (1024LL*1024LL*1024LL*8LL)

int main() {
	signal(SIGCHLD, SIG_IGN); // prevents zombies
	char wd_path[1024];
	getcwd(wd_path, sizeof(wd_path));
	int nodes;
	int option;
	printf("File scanning (1)\nBogosort (2)\nChoose the number program to test: ");
	scanf("%d", &option);
	if (!(option == 1 || option == 2)) {
		printf("You must select option 1 or option 2 by inputting the number 1 or the number 2.\n");
		return 1;
	}
	printf("How many nodes should be created? (1-35): ");
	scanf("%d", &nodes);
	if (nodes<1 || nodes>35) {
		printf("You must make between 1 and 35 nodes!\n");
		return 1;
	}
	int order;
	if (option == 2) {
		printf("What order of Bogosort should be tested? (Soft maximum is 13): ");
		scanf("%d", &order);
		if (order<2) {
			printf("Choose an order above 1!\n");
			return 1;
		}
		else if (order>13) printf("Good luck.\n");
	}
	struct timeval start_time, end_time;
	gettimeofday(&start_time, NULL);
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
	int unsorted[order];
	int seed;
	FILE* r_file;
	if (option == 2) {
		r_file = fopen("/dev/urandom", "rb");
		fread(unsorted, sizeof(int), order, r_file);
		fclose(r_file);
		printf("Unsorted array:\n[");
		for (int j = 0; j<order-1; j++) {
			printf("%d, ", unsorted[j]);
		}
		printf("%d]\n", unsorted[order-1]);
	}
	for (int i = 0; i<nodes; i++) {
		if (option == 1) {
			snprintf(cmd, sizeof(cmd), "%s/progn %s %s %d %lld %lld", wd_path, ip, wd_path, option, ((long long)i)*seg, (i==nodes-1)?(BYTES):((long long)(i+1))*seg);
		}
		else {
			r_file = fopen("/dev/urandom", "rb");
			fread(&seed, sizeof(int), 1, r_file);
			fclose(r_file);
			snprintf(cmd, sizeof(cmd), "%s/progn %s %s %d %d %d", wd_path, ip, wd_path, option, order, seed);
			for (int j = 0; j<order; j++) {
				snprintf(cmd+strlen(cmd), sizeof(cmd), " %d", unsorted[j]);
			}
		}
		snprintf(cmd2, sizeof(cmd2), "sisakov60@149.89.40.%d", 100+i);
		char* args[] = {"ssh", cmd2, cmd, 0};
		if (!fork()) execvp(args[0], args);
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
	//printf("All nodes connected.\n");
	/*
	for (int i = 0; i<nodes; i++) {
		char task[128];
		snprintf(task, sizeof(task), "Node %d echo", i);
		write(socks[i], task, strlen(task));
	}
	*/
	if (option == 1) {
		int max = INT_MIN;
		int nodes_finished = 0;
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
		gettimeofday(&end_time, NULL);
		double time_taken = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
		printf("Max is %d, took %f seconds.\n", max, time_taken);
	}
	else if (option == 2) {
		while (1) {
			select_fds = read_fds;
			int selret = select(fd_max + 1, &select_fds, 0, 0, 0);
			if (!selret) {
				printf("sel returned zero\n");
				return 1;
			}
			for (int i = 0; i<nodes; i++) {
				if (FD_ISSET(socks[i], &select_fds)) {
					int sorted[order];
					read(socks[i], sorted, sizeof(sorted));
					printf("Sorted array:\n[");
					for (int j = 0; j<order-1; j++) {
						printf("%d, ", sorted[j]);
					}
					printf("%d]\n", sorted[order-1]);
					gettimeofday(&end_time, NULL);
					double time_taken = (end_time.tv_sec - start_time.tv_sec) + (end_time.tv_usec - start_time.tv_usec) / 1000000.0;
					printf("Solved in %f seconds.\n", time_taken);
					//const char buffchar[] = "h";
					//close(socks[i]);
					for (int j = 0; j<nodes; j++) {
						//if (j == i) continue;
						//write(socks[j], buffchar, sizeof(buffchar));
						close(socks[j]);
					}
					return 0;
				}
			}
		}
	}
	return 0;
}
