#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <omp.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>

void dump_time(double io_t, double mul_t, double total_t) {
    FILE *f = fopen("time.txt", "w");
    assert(f);
    fprintf(f, "%f\n", io_t);
    fprintf(f, "%f\n", mul_t);
    fprintf(f, "%f\n", total_t);
    fclose(f);
}

void multiply_naive(int *a, int *b, int *c, int m, int n, int l) {
    # pragma omp parallel for schedule(dynamic, 1)
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
    # pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            int r = a[i * n + k];
            for (int j = 0; j < l; j++)
                c[i * l + j] += r * b[k * l + j];
        }
    }
}

void multiply_cache_friendly_sse_v1(int *a, int *b, int *c, int m, int n, int l) {
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

void multiply_cache_friendly_sse_v2(int *a, int *b, int *c, int m, int n, int l) {
    # pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < m; i++) {
        int k_end = n - (n % 4);
        for (int k = 0; k < k_end; k += 4) {
            int l_end = l - (l % 4);
            __m128i v_r = _mm_loadu_si128((__m128i *) &a[i * n + k]);

            for (int j = 0; j < l_end; j += 4) {
                __m128i v_c = _mm_loadu_si128((__m128i *) &c[i * l + j]);
                for(int p = 0; p < 4; p++) {
                    __m128i v_b = _mm_loadu_si128((__m128i *) &b[(k + p) * l + j]);
                    __m128i pth_v_a = _mm_shuffle_epi32(v_r, _MM_SHUFFLE(p, p, p, p)); // get the p-th element of v_r
                    v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(pth_v_a, v_b));
                }
                _mm_storeu_si128((__m128i *) &c[i * l + j], v_c);
            }

            if (l_end != l) {
                for (int j = l - (l % 4); j < l; j++)
                    for(int p = 0; p < 4; p++)
                        c[i * l + j] += a[i * n + (k + p)] * b[(k + p) * l + j];
            }
        }

        if(k_end != n) {
            for (int k = n - (n % 4); k < n; k++) {
                int r = a[i * n + k];
                for (int j = 0; j < l; j++)
                    c[i * l + j] += r * b[k * l + j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // Timer start
    double total_t, mul_t;
    total_t = omp_get_wtime();

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
    mul_t = omp_get_wtime();
    multiply_cache_friendly_sse_v2(a, b, c, m, n, l);
    mul_t = omp_get_wtime() - mul_t;
    
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
    total_t = omp_get_wtime() - total_t;
    printf("Total time: %f seconds\n", total_t);
    printf("CPU time: %f seconds\n", mul_t);
    printf("IO time: %f seconds\n", total_t - mul_t);
    dump_time(total_t - mul_t, mul_t, total_t);

    return 0;
}
