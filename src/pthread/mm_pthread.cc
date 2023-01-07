#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>

int m, n, l;
int *a, *b, *c;
pthread_mutex_t mutex;
int row_num = 0;

void multiply_naive(int row){
    for (int j = 0; j < l; j++) {
        int sum = 0;
        for (int k = 0; k < n; k++)
            sum += a[row * n + k] * b[k * l + j];
        c[row * l + j] = sum;
    }
}

void multiply_cache_friendly(int row){
    for (int k = 0; k < n; k++) {
        int r = a[row * n + k];
        for (int j = 0; j < l; j++)
            c[row * l + j] += r * b[k * l + j];
    }
}

void multiply_sse(int row){
    for (int k = 0; k < n; k++) {
        int end = l - (l % 4);
        int r = a[row * n + k];
        __m128i v_r = _mm_set1_epi32(r);

        for (int j = 0; j < end; j += 4) {
            __m128i v_c = _mm_loadu_si128((__m128i *) &c[row * l + j]);
            __m128i v_b = _mm_loadu_si128((__m128i *) &b[k * l + j]);
            v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(v_r, v_b));
            _mm_storeu_si128((__m128i *) &c[row * l + j], v_c);
        }

        if (end != l) {
            for (int j = l - (l % 4); j < l; j++)
                c[row * l + j] += r * b[k * l + j];
        }
    }
}

void multiply_sse_v2(int row){
    int k_end = n - (n % 4);
    for (int k = 0; k < k_end; k += 4) {
        int l_end = l - (l % 4);
        __m128i v_r = _mm_loadu_si128((__m128i *) &a[row * n + k]);

        for (int j = 0; j < l_end; j += 4) {
            __m128i v_c = _mm_loadu_si128((__m128i *) &c[row * l + j]);
            for(int p = 0; p < 4; p++) {
                __m128i v_b = _mm_loadu_si128((__m128i *) &b[(k + p) * l + j]);
                __m128i pth_v_a = _mm_shuffle_epi32(v_r, _MM_SHUFFLE(p, p, p, p)); // get the p-th element of v_r
                v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(pth_v_a, v_b));
            }
            _mm_storeu_si128((__m128i *) &c[row * l + j], v_c);
        }

        if (l_end != l) {
            for (int j = l - (l % 4); j < l; j++)
                for(int p = 0; p < 4; p++)
                    c[row * l + j] += a[row * n + (k + p)] * b[(k + p) * l + j];
        }
    }

    if(k_end != n) {
        for (int k = n - (n % 4); k < n; k++) {
            int r = a[row * n + k];
            for (int j = 0; j < l; j++)
                c[row * l + j] += r * b[k * l + j];
        }
    }
}

void* matrix_multiply(void*){
    int row_to_calc;
    while (true){
        pthread_mutex_lock(&mutex);
        row_to_calc = row_num;
        row_num++;
        pthread_mutex_unlock(&mutex);
        if (row_to_calc >= m)
            break;
        else
            multiply_sse_v2(row_to_calc);
    }
    pthread_exit(NULL);
}

double cal_time(timespec start, timespec end){
    return ((double)(end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)) / 1000000000;
}

int main(int argc, char *argv[]) {
    // Timer start
    struct timespec prog_start = {0, 0};
    struct timespec prog_end = {0, 0};
    struct timespec cpu_start = {0, 0};
    struct timespec cpu_end = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &prog_start);

    // Read inputs
    assert(argc == 3);
    FILE *f = fopen(argv[1], "r");
    assert(f);
    fscanf(f, "%d %d %d", &m, &n, &l);
    a = new int[m * n];
    b = new int[n * l];
    c = new int[m * l];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &a[i * n + j]);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < l; j++)
            fscanf(f, "%d", &b[i * l + j]);
    fclose(f);


    // Detect how many CPUs are available
    clock_gettime(CLOCK_MONOTONIC, &cpu_start);
    cpu_set_t cpu_set;
    sched_getaffinity(0, sizeof(cpu_set), &cpu_set);
    int ncpus = CPU_COUNT(&cpu_set);
    printf("%d cpus available\n", CPU_COUNT(&cpu_set));

    // pthread parallel
    pthread_t threads[ncpus];
    pthread_mutex_init(&mutex, NULL);
    for (int t=0; t<ncpus; t++) 
        pthread_create(&threads[t], NULL, matrix_multiply, NULL);
	for (int t=0; t<ncpus; t++)
		pthread_join(threads[t], NULL);	
    
    clock_gettime(CLOCK_MONOTONIC, &cpu_end);


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
    clock_gettime(CLOCK_MONOTONIC, &prog_end);
    double total_time = cal_time(prog_start, prog_end);
    double cpu_time = cal_time(cpu_start, cpu_end);
    printf("Total time: %f seconds\n", total_time);
    printf("CPU time: %f seconds\n", cpu_time);
    printf("IO time: %f seconds\n", total_time - cpu_time);

    return 0;
}
