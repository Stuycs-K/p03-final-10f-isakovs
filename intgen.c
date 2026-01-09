#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BYTES (1024LL*1024LL*1024LL*8LL)
#define KB64 65536

int main() {
    FILE *r_file = fopen("/dev/urandom", "rb");
    FILE *out_file = fopen("random_numbers.txt", "w");
	char out[KB64];
	setvbuf(out_file, out, _IOFBF, KB64);
    long long written = 0;
    int random_ints[4096];
    while (written < BYTES) {
	    int bytesread = fread(random_ints, sizeof(int), 4096, r_file);
	    for (int i = 0; i<bytesread && written<BYTES; i++) {
		    int chars;
		    if ((chars = fprintf(out_file, "%d\n", random_ints[i])) > 0) written += chars;
	    }
    }
    fclose(out_file);
    fclose(r_file);
	printf("The random integer file has been generated.\n");
    return 0;
}
