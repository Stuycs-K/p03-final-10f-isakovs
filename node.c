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
    	memset(buff, 0, BUFFER_SIZE);
    	char cmd[2048];
    	snprintf(cmd, sizeof(cmd), "%s/progmax %s %s %s 2>&1", argv[2], argv[2], argv[3], argv[4]);
    	FILE* fp = popen(cmd, "r");
    	if (fgets(buff, sizeof(buff), fp)) {
    		write(server_socket, buff, strlen(buff));
    	} else printf("empty pipe\n");
    	pclose(fp);
    }
    else if (option == 2) {
        int order = atoi(argv[4]);
		int seed = atoi(argv[5]);
		int a[order];
		for (int i = 0; i<order; i++) {
			a[i] = atoi(argv[6+i]);
		}
		srand(seed);
		
		struct pollfd pfd;
		pfd.fd = server_socket;
		pfd.events = POLLIN | POLLHUP;
		pfd.revents = 0;
		while (1) {
			if (poll(&pfd, 1, 0)) {
				//printf("Node closed to poll\n");
				return 0;
			}
			for (int useless = 0; useless<25000000; useless++) {
				int t;
				int r;
				int sorted = 1;
				for (int i = 0; i<order-1; i++) {
					if (a[i]>a[i+1]) {
						sorted = 0;
						break;
					}
				}
				if (sorted) {
					write(server_socket, a, sizeof(a));
					/*printf("Written: [");
					for (int j = 0; j<order-1; j++) {
						printf("%d, ", a[j]);
					}
					printf("%d]\n", a[order-1]);*/
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
    } else if (option == 3) {
		int order = atoi(argv[4]);
		int seed = atoi(argv[5]);
		srand48(seed);
		struct pollfd pfd;
		pfd.fd = server_socket;
		pfd.events = POLLIN | POLLHUP;
		pfd.revents = 0;
		
		long circle = 0L;
		long points_per_order = 100000000L;
		long points = (long)order * points_per_order;
		long poll_limit = points_per_order;
		long p = poll_limit;
		double x, y;
		for (long i = 0; i<points; i++) {
			if (p == poll_limit) {
				if (poll(&pfd, 1, 0)) return 0;
				p = 0L;
			}
			x = drand48();
			y = drand48();
			if (x*x + y*y <= 1.0) circle++;
			p++;
		}
		long double approx = 4.0L*((long double)circle / (long double)points);
		//printf("Approx: %f, Circle: %d\n", approx, circle);
		write(server_socket, &approx, sizeof(long double));
	}
	return 0;
}
