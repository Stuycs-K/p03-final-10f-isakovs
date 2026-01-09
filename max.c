#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (!argv[1]) {
        int begin = clock();
        FILE *file = fopen("random_numbers.txt", "r");
        int max = INT_MIN;
        int curr;
        char buff[64];
        while (fgets(buff, 64, file)) {
            curr = atoi(buff);
            if (max < curr) max = curr;
        }
        fclose(file);
        printf("Max is %d, Took %f sec\n", max, (double)(clock()-begin)/CLOCKS_PER_SEC);
        return 0;
    }

    long long start = atoll(argv[2]);
    long long end = atoll(argv[3]);
	char path[1024];
	snprintf(path, sizeof(path), "%s/random_numbers.txt", argv[1]);
    FILE *file = fopen(path, "r");
    if (!file) return 1;
    fseeko(file, start, SEEK_SET);
    char buff[64];
    if (start != 0) fgets(buff, 64, file);
    int max = INT_MIN;
    int curr;
    while (ftello(file) < end && fgets(buff, 64, file)) {
        curr = atoi(buff);
        if (max < curr) max = curr;
    }
    fclose(file);
    printf("%d\n", max);
    return 0;
}
