#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BYTES_PER_GB (1024LL*1024LL*1024LL)

static void err() {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(1);
}

int main() {
    int r_file = open("/dev/urandom", O_RDONLY);
    if (r_file == -1) err();
    FILE *out_file = fopen("random_numbers.txt", "w");
    if (!out_file) err();
    long long bytes_written = 0;
    int random_val;
    while (bytes_written < BYTES_PER_GB) {
        if (read(r_file, &random_val, sizeof(int)) != sizeof(int)) err();
        int char_count = fprintf(out_file, "%d\n", random_val);
        if (char_count < 0) err();
        bytes_written += char_count;
    }
    fclose(out_file);
    close(r_file);
	printf("done\n");
    return 0;
}
