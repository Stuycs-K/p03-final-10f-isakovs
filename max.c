#include <stdio.h>
#include <limits.h>
#include <time.h>

int main() {
	int begin = clock();
    FILE *file = fopen("random_numbers.txt", "r");
    int max = INT_MIN;
    int curr;
    while (fscanf(file, "%d", &curr)) {
        if (max < curr) max = curr;
    }
    fclose(file);
	printf("Max is %d, Took %f sec\n", max, (double)(clock()-begin)/CLOCKS_PER_SEC);
    return 0;
}
