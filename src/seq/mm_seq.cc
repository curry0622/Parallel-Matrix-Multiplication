#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <pmmintrin.h>

#include <cstring>

// 用lseek要include這兩個
#include <sys/types.h>
#include <unistd.h>
// 用mmapio要include這兩個
#include <sys/mman.h>
#include <fcntl.h>

int m, n, l;
int *a, *b, *c;

void input_original(char *filename){
    FILE *f = fopen(filename, "r");
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
}

void input_v2(char *filename){     // can't use
    FILE *f = fopen(filename, "r");
    assert(f);
    int fd = open(filename, O_RDONLY);
    off_t totoal_num_of_bytes = lseek(fd, 0, SEEK_END);
    printf("File size is %ld bytes.\n", totoal_num_of_bytes);

    int datas[totoal_num_of_bytes+1];
    char readDatas[totoal_num_of_bytes+1];
    char seps[] = " ";
    char *token = NULL;
    int i = 0;

    memset(readDatas, 0x0, sizeof(readDatas));
    fread(readDatas, sizeof(char), totoal_num_of_bytes, f);
    // readDatas[totoal_num_of_bytes-1] = 0x0;   // 不知道這行有什麼作用
    fclose(f);

    token = strtok(readDatas, seps);
    while(token != NULL){
        datas[i] = atoi(token);
        i++;
        token = strtok(NULL, seps);
    }

    m = datas[0]; n = datas[1]; l = datas[2];
    a = new int[m * n];
    b = new int[n * l];
    c = new int[m * l];
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            a[i * n + j] = datas[i * n + j + 3];
    for (int i=0; i<n; i++)
        for (int j = 0; j < l; j++)
            b[i * l + j] = datas[i * l + j + m * n + 3];
}

void input_v3(char *filename){
    int fd = open(filename, O_RDONLY);
    off_t sz = lseek(fd, 0, SEEK_END);
    printf("File size is %ld bytes.\n", sz);

    char *buf = (char*) mmap(NULL, sz, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    char *current_char = buf;
    long long current_num = 0;

    int datas[sz];
    int count = 0;
    bool saved = false;
    while (*current_char != '\0'){ // 字符串結尾標示'\0'
        if (*current_char != ' ' && *current_char != '\n'){
            current_num = 10 * current_num + *current_char - '0';  // 每次取出數字第一位
            saved = false;
        }
        else if (saved == false){ 
            // printf("current_num = %d\n", current_num);
            datas[count] = current_num;
            current_num = 0;
            count++;
            saved = true;
        }
        current_char++;
    }

    m = datas[0]; n = datas[1]; l = datas[2];
    a = new int[m * n];
    b = new int[n * l];
    c = new int[m * l];
    for (int i=0; i<m; i++)
        for (int j=0; j<n; j++)
            a[i * n + j] = datas[i * n + j + 3];
    for (int i=0; i<n; i++)
        for (int j = 0; j < l; j++)
            b[i * l + j] = datas[i * l + j + m * n + 3];

    // // print matrixs
    // printf("m = %d, n = %d, l = %d\n", m, n, l);
    // printf("matrix a: \n");
    // for (int i=0; i<m; i++){
    //     for (int j=0; j<n; j++)
    //         printf("%d ", a[i * n + j]);
    //     printf("\n");
    // }
    // printf("matrix b: \n");
    // for (int i=0; i<n; i++){
    //     for (int j = 0; j < l; j++)
    //         printf("%d ", b[i * l + j]);
    //     printf("\n");
    // }
}

void multiply_naive(int *a, int *b, int *c, int m, int n, int l) {
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
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            int r = a[i * n + k];
            for (int j = 0; j < l; j++)
                c[i * l + j] += r * b[k * l + j];
        }
    }
}

void multiply_cache_friendly_sse_v1(int *a, int *b, int *c, int m, int n, int l) {
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
    clock_t prog_t, cpu_t;
    prog_t = clock();

    // Read inputs
    assert(argc == 3);
    input_v2(argv[1]);

    // multiply a and b
    cpu_t = clock();
    multiply_cache_friendly_sse_v2(a, b, c, m, n, l);
    cpu_t = clock() - cpu_t;
    
    // Output c to file
    FILE *f = fopen(argv[2], "w");
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
