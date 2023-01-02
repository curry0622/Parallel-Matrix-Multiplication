#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>

void multiply_naive(int *a, int *b, int *c, int m, int n, int l) {
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < l; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++)
                sum += a[i * n + k] * b[k * l + j];
            c[i * l + j] = sum;
        }
    }
}

void multiply_cache_friendly(int *a, int *b, int *c, int m, int n, int l) {
    #pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            int r = a[i * n + k];
            for (int j = 0; j < l; j++)
                c[i * l + j] += r * b[k * l + j];
        }
    }
}

void multiply_sse(int *a, int *b, int *c, int m, int n, int l) {
    # pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            int end = l - (l % 4);
            int r = a[i * n + k];
            __m128i v_r = _mm_set1_epi32(r);

            for (int j = 0; j < end; j += 4) {
                __m128i v_c = _mm_loadu_si128((__m128i *) &c[i * l + j]);
                __m128i v_b = _mm_loadu_si128((__m128i *) &b[k * l + j]);
                v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(v_r, v_b));
                _mm_storeu_si128((__m128i *) &c[i * l + j], v_c);
            }

            if (end != l) {
                for (int j = l - (l % 4); j < l; j++)
                    c[i * l + j] += r * b[k * l + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Timer start
    double prog_t, cpu_t;
    prog_t = omp_get_wtime();

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
    cpu_t = omp_get_wtime();
    multiply_cache_friendly(a, b, c, m, n, l);
    cpu_t = omp_get_wtime() - cpu_t;
    
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
    prog_t = omp_get_wtime() - prog_t;
    printf("Total time: %f seconds\n", prog_t);
    printf("CPU time: %f seconds\n", cpu_t);
    printf("IO time: %f seconds\n", prog_t - cpu_t);

    return 0;
}
