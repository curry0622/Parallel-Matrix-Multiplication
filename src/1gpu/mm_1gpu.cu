#include <stdio.h>
#include <assert.h>
#include <time.h>

void multiply_cache_friendly(int *a, int *b, int *c, int m, int n, int l) {
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            int r = a[i * n + k];
            for (int j = 0; j < l; j++)
                c[i * l + j] += r * b[k * l + j];
        }
    }
}

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
    int *a = new int[m * n];
    int *b = new int[n * l];
    int *c = new int[m * l];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &a[i * n + j]);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < l; j++)
            fscanf(f, "%d", &b[i * l + j]);
    fclose(f);

    // multiply a and b
    cpu_t = clock();
    multiply_cache_friendly(a, b, c, m, n, l);
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
