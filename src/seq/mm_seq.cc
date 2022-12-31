#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
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

    // Multiply a and b
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            c[i * l + j] = 0;
            for (int k = 0; k < n; k++)
                c[i * l + j] += a[i * n + k] * b[k * l + j];
        }
    }
    
    // Output c to file
    f = fopen(argv[2], "w");
    assert(f);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++)
            fprintf(f, "%d ", c[i * l + j]);
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}
