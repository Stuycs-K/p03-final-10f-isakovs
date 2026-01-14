#include "networking.h"

void clientLogic(int server_socket){
    }

int main(int argc, char *argv[]) {
	char* IP;
	if (argv[1]) IP = argv[1];
	else IP = "127.0.0.1";
    int server_socket = client_tcp_handshake(IP);
	int option = atoi(argv[3]);
    if (option == 1) {
        char buff[BUFFER_SIZE];
        //while (1) {
    	memset(buff, 0, BUFFER_SIZE);
    	char cmd[2048];
    	snprintf(cmd, sizeof(cmd), "%s/progmax %s %s %s 2>&1", argv[2], argv[2], argv[3], argv[4]);
    	FILE* fp = popen(cmd, "r");
    	//printf("Node: Executing command \n");
    	if (fgets(buff, sizeof(buff), fp)) {
    		//printf("Node: Read value '%s' from pipe\n", buff);
    		write(server_socket, buff, strlen(buff));
    		//printf("Node: Sent data to master\n");
    	} else printf("empty pipe\n");
    	pclose(fp);
    		/*memset(buff, 0, BUFFER_SIZE);
            if (!read(server_socket, buff, BUFFER_SIZE-1)) {
    			exit(0);
    		}
    		buff[strlen(buff)] = 0;
    		write(server_socket, buff, strlen(buff));
    		*/
        //}
    }
    else if (option == 2) {
        int order = atoi(argv[4]);
		int seed = atoi(argv[5]);
		int a[order];
		for (int i = 0; i<order; i++) {
			a[i] = atoi(argv[6+i]);
		}
		srand(seed);

		fd_set read_fds;
		fd_set select_fds;
		FD_ZERO(&read_fds);
		FD_SET(server_socket);
		int selret;
		while (1) { // Replace 1 with ping()
			select_fds = read_fds;
			selret = select(server_socket+1, &select_fds, 0, 0, 0);
			if (FD_ISSET(server_socket, &select_fds)) return 0;
			for (int useless = 0; useless<10000; useless++) {
				int t;
				int r;
				int sorted = 1;
				for (int i = 0; i<order; i++) {
					if (a[i]>a[i+1]) {
						sorted = 0;
						break;
					}
				}
				if (sorted) {
					write(server_socket, a, sizeof(a));
					return 0;
				}
				for (int i = 0; i<order; i++) {
					r = rand() % order;
					t = a[r];
					a[r] = a[i];
					a[i] = t;
				}
			}
		}
		// ADD ping() in networking.c/h and ping every 100k iter.

    }
	return 0;
}
