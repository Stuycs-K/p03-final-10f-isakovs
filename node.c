#include "networking.h"

void clientLogic(int server_socket){
    }

int main(int argc, char *argv[]) {
	char* IP;
	if (argv[1]) IP = argv[1];
	else IP = "127.0.0.1";
    int server_socket = client_tcp_handshake(IP);
    char buff[BUFFER_SIZE];
    //while (1) {
	memset(buff, 0, BUFFER_SIZE);
	char cmd[2048];
	snprintf(cmd, sizeof(cmd), "%s/progmax %s %s %s 2>&1", argv[2], argv[2], argv[3], argv[4]);
	FILE* fp = popen(cmd, "r");
	if (fgets(buff, sizeof(buff), fp)) {
		write(server_socket, buff, strlen(buff));
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
	return 0;
}
