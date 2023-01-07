#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <mpi.h>
#include <cmath>
#include <emmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>

int m, n, l;
int *a, *b, *c_part, *c;

void multiply_naive(int row, int start){
    int new_row = row - start;
    for (int j = 0; j < l; j++) {
        int sum = 0;
        for (int k = 0; k < n; k++)
            sum += a[row * n + k] * b[k * l + j];
        c_part[new_row * l + j] = sum;
    }
}

void multiply_cache_friendly(int row, int start){
    int new_row = row - start;
    for (int k = 0; k < n; k++) {
        int r = a[row * n + k];
        for (int j = 0; j < l; j++)
            c_part[new_row * l + j] += r * b[k * l + j];
    }
}

void multiply_sse(int row, int start){
    int new_row = row - start;
    for (int k = 0; k < n; k++) {
        int end = l - (l % 4);
        int r = a[row * n + k];
        __m128i v_r = _mm_set1_epi32(r);

        for (int j = 0; j < end; j += 4) {
            __m128i v_c = _mm_loadu_si128((__m128i *) &c_part[new_row * l + j]);
            __m128i v_b = _mm_loadu_si128((__m128i *) &b[k * l + j]);
            v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(v_r, v_b));
            _mm_storeu_si128((__m128i *) &c_part[new_row * l + j], v_c);
        }

        if (end != l) {
            for (int j = l - (l % 4); j < l; j++)
                c_part[new_row * l + j] += r * b[k * l + j];
        }
    }
}

void multiply_sse_v2(int row, int start){
    int new_row = row - start;
    int k_end = n - (n % 4);
    for (int k = 0; k < k_end; k += 4) {
        int l_end = l - (l % 4);
        __m128i v_r = _mm_loadu_si128((__m128i *) &a[row * n + k]);

        for (int j = 0; j < l_end; j += 4) {
            __m128i v_c = _mm_loadu_si128((__m128i *) &c_part[new_row * l + j]);
            for(int p = 0; p < 4; p++) {
                __m128i v_b = _mm_loadu_si128((__m128i *) &b[(k + p) * l + j]);
                __m128i pth_v_a = _mm_shuffle_epi32(v_r, _MM_SHUFFLE(p, p, p, p)); // get the p-th element of v_r
                v_c = _mm_add_epi32(v_c, _mm_mullo_epi32(pth_v_a, v_b));
            }
            _mm_storeu_si128((__m128i *) &c_part[new_row * l + j], v_c);
        }

        if (l_end != l) {
            for (int j = l - (l % 4); j < l; j++)
                for(int p = 0; p < 4; p++)
                    c_part[new_row * l + j] += a[row * n + (k + p)] * b[(k + p) * l + j];
        }
    }

    if(k_end != n) {
        for (int k = n - (n % 4); k < n; k++) {
            int r = a[row * n + k];
            for (int j = 0; j < l; j++)
                c_part[new_row * l + j] += r * b[k * l + j];
        }
    }
}


void calculate_rank_rows(int* info, int m, int size, int rank){
	int rank_rows = m / size;
	int remainder = m % size;
	int start;
	if (rank < remainder){   // will have one more data
		rank_rows += 1;
		start = rank_rows * rank;
	} 
    else
		start = rank_rows * rank + remainder;
	info[0] = rank_rows;
	info[1] = start;
}

void calculate_gatherv(int m, int size, int* counts, int* displacements){
    int temp[2];
    for (int i=0; i<size; i++){
        calculate_rank_rows(temp, m, size, i);
        counts[i] = temp[0] * l;
        displacements[i] = temp[1] * l;
    }
}


int main(int argc, char *argv[]) {
    // MPI
    MPI_Init(&argc, &argv);
	int root_rank = 0;
	int my_rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Timer start
    double prog_t, cpu_t;
    prog_t = MPI_Wtime();

    // Read inputs
    assert(argc == 3);
    FILE *f = fopen(argv[1], "r");
    assert(f);
    fscanf(f, "%d %d %d", &m, &n, &l);
    a = new int[m * n];
    b = new int[n * l];
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            fscanf(f, "%d", &a[i * n + j]);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < l; j++)
            fscanf(f, "%d", &b[i * l + j]);
    fclose(f);

    // allocate memory for c_part
    cpu_t = MPI_Wtime();
    int my_info[2];  // rank_rows, start
    calculate_rank_rows(my_info, m, size, my_rank);
    int rank_rows = my_info[0];
    int start = my_info[1];

    c_part = new int[rank_rows * l]();
    for (int i=start; i<start+rank_rows; i++)
        multiply_sse_v2(i, start);

    if(my_rank == root_rank){
        c = new int[m * l];
        assert(c);
        int counts[size]; // Define the receive counts
        int displacements[size];  // Define the displacements
        calculate_gatherv(m, size, counts, displacements);
        MPI_Gatherv(c_part, rank_rows * l, MPI_INT, c, counts, displacements, MPI_INT, root_rank, MPI_COMM_WORLD);
        cpu_t = MPI_Wtime() - cpu_t;

        // Output c to file
        f = fopen(argv[2], "w");
        assert(f);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < l; j++)
                fprintf(f, "%ld ", c[i * l + j]);
            fprintf(f, "\n");
        }
        fclose(f);
    }
    else{
        MPI_Gatherv(c_part, rank_rows * l, MPI_INT, NULL, NULL, NULL, MPI_INT, root_rank, MPI_COMM_WORLD);
        cpu_t = MPI_Wtime() - cpu_t;
    }

    if (my_rank == root_rank){
        // Timer end
        prog_t = MPI_Wtime() - prog_t;
        printf("Total time: %f seconds\n", prog_t);
        printf("CPU time: %f seconds\n", cpu_t);
        printf("IO time: %f seconds\n", prog_t - cpu_t);
    }
    MPI_Finalize();
}
