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
	} else if (option == 4) {
		int max_iter = atoi(argv[4]);
		int width = atoi(argv[5]);
		int height_lower = atoi(argv[6]);
		int height_upper = atoi(argv[7]);
		int coordset = atoi(argv[8]);

		char filepath[256];
		snprintf(filepath, sizeof(filepath), "%s/mandelbrot.ppm", argv[2]);
		FILE *fp = fopen(filepath, "rb+");
		if (fp == NULL) {
			return 1;
		}
		char header[128];
		int header_offset = sprintf(header, "P6\n%d %d\n255\n", width, width);
		long start_pos = (long)header_offset + ((long)height_lower * width * 3);
		fseek(fp, start_pos, SEEK_SET);
		double zoom, center_x, center_y;
		if (coordset == 2) {
			// seahorse
			zoom = 200.0;
			center_x = -0.74364;
			center_y = 0.13182;
		} else if (coordset == 3) {
			// elephant
			zoom = 50.0;
			center_x = 0.3;
			center_y = 0.01;
		} else if (coordset == 4) {
			// triple spiral
			zoom = 1000.0;
			center_x = -0.088;
			center_y = 0.654;
		}
		else {
			// regular
			zoom = 1.0;
			center_x = -0.5;
			center_y = 0.0;
		}
		for (int y = height_lower; y < height_upper; y++) {
				for (int x = 0; x < width; x++) {
						double cr = center_x + (x - width / 2.0) * (3.0 / (width * zoom));
						double ci = center_y + (y - 800 / 2.0) * (3.0 / (800 * zoom));
						double zr = 0, zi = 0;
						int iter = 0;
						while (zr * zr + zi * zi <= 4.0 && iter < max_iter) {
							double temp = zr * zr - zi * zi + cr;
							zi = 2.0 * zr * zi + ci;
							zr = temp;
							iter++;
						}
						unsigned char r, g, b;
						if (iter == max_iter) {
							r = 0; g = 0; b = 0;
						} else {
							r = (unsigned char)((iter * 10) % 256);
							g = (unsigned char)((iter * 5) % 256);
								b = (unsigned char)((iter * 2) % 256);
						}
						fputc(r, fp);
						fputc(g, fp);
						fputc(b, fp);
				}
		}
		fflush(fp);
		fclose(fp);
		write(server_socket, &width, sizeof(int));


	}

	return 0;
}
