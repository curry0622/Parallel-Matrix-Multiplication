#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

int m, n, l;
long *a, *b, *c;

struct Range{
	int start;
	int end;
};

void* multiply_naive(void* arg){
    Range* range = (Range*) arg;     // get range
    for (int i = range->start; i < range->end; i++) {
        for (int j = 0; j < l; j++) {
            c[i * l + j] = 0;
            for (int k = 0; k < n; k++)
                c[i * l + j] += a[i * n + k] * b[k * l + j];
        }
    }
    pthread_exit(NULL);
}

void* multiply_cache_friendly(void* arg){
    Range* range = (Range*) arg;     // get range
    for (int i = range->start; i < range->end; i++) {
        for (int k = 0; k < n; k++) {
            int r = a[i * n + k];
            for (int j = 0; j < l; j++)
                c[i * l + j] += r * b[k * l + j];
        }
    }
    pthread_exit(NULL);
}

Range cal_range(int n, int ncpus, int thread_ID){
    Range range = Range();
    int chunk_size = n / ncpus;
    int remainder = n % ncpus;
    int start;

    if (thread_ID < remainder){
        start = chunk_size * thread_ID + thread_ID;
        chunk_size += 1;
    }
    else
        start = chunk_size * thread_ID + remainder;
    
    range.start = start;
    range.end = start + chunk_size - 1;    // end is concluded
    return range;
}

double cal_time(timespec start, timespec end){
    return ((double)(end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec)) / 1000000000;
}

int main(int argc, char *argv[]) {
    // Timer start
    // clock_t prog_t, cpu_t;
    // prog_t = clock();

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
    a = new long[m * n];
    b = new long[n * l];
    c = new long[m * l];
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
    for (int t=0; t<ncpus; t++) {
        Range range = cal_range(n, ncpus, t);
        pthread_create(&threads[t], NULL, multiply_cache_friendly, (void*) &range);
    }
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
    // prog_t = clock() - prog_t;
    // printf("Total time: %f seconds\n", (double)prog_t / CLOCKS_PER_SEC);
    // printf("CPU time: %f seconds\n", (double)cpu_t / CLOCKS_PER_SEC);
    // printf("IO time: %f seconds\n", (double)(prog_t - cpu_t) / CLOCKS_PER_SEC);

    clock_gettime(CLOCK_MONOTONIC, &prog_end);
    double total_time = cal_time(prog_start, prog_end);
    double cpu_time = cal_time(cpu_start, cpu_end);
    printf("Total time: %f seconds\n", total_time);
    printf("CPU time: %f seconds\n", cpu_time);
    printf("IO time: %f seconds\n", total_time - cpu_time);

    return 0;
}
