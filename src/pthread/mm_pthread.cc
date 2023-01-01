#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

int main(int argc, char *argv[]) {
    // Timer start
    clock_t prog_t, cpu_t;
    prog_t = clock();

    // Read inputs
    assert(argc == 3);
    FILE *f = fopen(argv[1], "r");
    assert(f);
    int m, n, l;
    fscanf(f, "%d %d %d", &m, &n, &l);
    long *a = new long[m * n];
    long *b = new long[n * l];
    long *c = new long[m * l];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &a[i * n + j]);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < l; j++)
            fscanf(f, "%d", &b[i * l + j]);
    fclose(f);

    // Multiply a and b
    cpu_t = clock();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            c[i * l + j] = 0;
            for (int k = 0; k < n; k++)
                c[i * l + j] += a[i * n + k] * b[k * l + j];
        }
    }
    cpu_t = clock() - cpu_t;
    
    // Output c to file
    f = fopen(argv[2], "w");
    assert(f);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++)
            fprintf(f, "%ld ", c[i * l + j]);
        fprintf(f, "\n");
    }
    fclose(f);

    // Timer end
    prog_t = clock() - prog_t;
    printf("Total time: %f seconds\n", (double)prog_t / CLOCKS_PER_SEC);
    printf("CPU time: %f seconds\n", (double)cpu_t / CLOCKS_PER_SEC);
    printf("IO time: %f seconds\n", (double)(prog_t - cpu_t) / CLOCKS_PER_SEC);

    return 0;
}
