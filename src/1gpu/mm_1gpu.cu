#include <stdio.h>
#include <assert.h>
#include <time.h>

#define B 32

int *a, *b, *c;
int m, n, l;

void input(char *filename) {
    // Open file
    FILE *f = fopen(filename, "r");
    assert(f);

    // Read m, n, l
    fscanf(f, "%d %d %d", &m, &n, &l);

    // Allocate memory
    cudaMallocHost((void**)&a, m * n * sizeof(int));
    cudaMallocHost((void**)&b, n * l * sizeof(int));
    cudaMallocHost((void**)&c, m * l * sizeof(int));

    // Read a
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &a[i * n + j]);

    // Read b
    for (int i = 0; i < n; i++)
        for (int j = 0; j < l; j++)
            fscanf(f, "%d", &b[i * l + j]);

    // Close file
    fclose(f);
}

void output(char *filename) {
    // Open file
    FILE *f = fopen(filename, "w");
    assert(f);

    // Write c
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++)
            fprintf(f, "%d ", c[i * l + j]);
        fprintf(f, "\n");
    }

    // Close file
    fclose(f);
}

__global__ void multiply_naive(int *d_a, int *d_b, int *d_c, int m, int n, int l) {
    int row_idx = blockIdx.y * blockDim.y + threadIdx.y;
    int col_idx = blockIdx.x * blockDim.x + threadIdx.x;
    if(row_idx >= m || col_idx >= l) return;
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum += d_a[row_idx * n + i] * d_b[i * l + col_idx];
    d_c[row_idx * l + col_idx] = sum;
}

int main(int argc, char *argv[]) {
    // Timer start
    clock_t prog_t, input_t, output_t;
    prog_t = clock();

    // Argument check
    assert(argc == 3);

    // Read inputs
    input_t = clock();
    input(argv[1]);
    input_t = clock() - input_t;

    // Allocate memory on device
    int *d_a, *d_b, *d_c;
    cudaMalloc((void**)&d_a, m * n * sizeof(int));
    cudaMalloc((void**)&d_b, n * l * sizeof(int));
    cudaMalloc((void**)&d_c, m * l * sizeof(int));

    // Copy inputs to device
    cudaMemcpy(d_a, a, m * n * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, n * l * sizeof(int), cudaMemcpyHostToDevice);

    // Multiply a and b
    printf("Multiplying a and b on GPU...\n");
    multiply_naive<<<dim3(l / B + 1, m / B + 1), dim3(B, B)>>>(d_a, d_b, d_c, m, n, l);
    printf("done\n");

    // Copy result from device to host
    cudaMemcpy(c, d_c, m * l * sizeof(int), cudaMemcpyDeviceToHost);
    
    // Output c to file
    output_t = clock();
    output(argv[2]);
    output_t = clock() - output_t;

    // Print time
    prog_t = clock() - prog_t;
    printf("Time: %f\n", (double)prog_t / CLOCKS_PER_SEC);
    printf("IO time: %f\n", (double)(input_t + output_t) / CLOCKS_PER_SEC);
    printf("GPU time: %f\n", (double)(prog_t - input_t - output_t) / CLOCKS_PER_SEC);

    return 0;
}
