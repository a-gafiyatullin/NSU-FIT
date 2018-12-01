#include <stdio.h>      //printf
#include <string.h>     //memset
#include <time.h>       //time
#include <cblas.h>      //cblas_scopy, cblas_sscal, cblas_sgemm
#include <sys/times.h>  //times
#include <unistd.h>     //sysconf
#include <math.h>       //fabsf
#include <xmmintrin.h>  //_mm_malloc
#define  ALIGN 16

float* gen_matrix(int order, float range){
    float* m = _mm_malloc(order * order * sizeof(float), ALIGN);
    srand(time(NULL));
    for(int i = 0; i < order * order; ++i)
        m[i] = ((float)rand() / (float)(RAND_MAX)) * range;
    return m;
}

void transpose_matrix(float* m, int order){
    for(int i = 0; i < order; ++i)
        for(int j = i; j < order; ++j){
            float tmp = m[i * order + j];
            m[i * order + j] = m[j * order + i];
            m[j * order + i] = tmp;
        }
}

void print_matrix(float* m, int order, FILE* out){
    fprintf(out, "Order: %d\n", order);
    for(int i = 0; i < order; ++i){
        for(int j = 0; j < order; ++j)
            fprintf(out, "%.4f ", m[i * order + j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void get_matrix_norms(float* m, int order, float* l1_norm, float* max_norm){
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < order; ++i){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < order; ++j){
            row_sum += fabs(m[i * order + j]);
            col_sum += fabs(m[j * order + i]);
        }
        if(*l1_norm < col_sum)
            *l1_norm = col_sum;
        if(*max_norm < row_sum)
            *max_norm = row_sum;
    }
}

float* get_identity_matrix(int order){
    float* m = _mm_malloc(order * order * sizeof(float), ALIGN);
    memset(m, 0, order * order * sizeof(float));
    for(int i = 0; i < order; ++i)
        m[i * order + i] = 1;
    return m;
}

float* invert_matrix(float* A, int order, int iter_number){
    float* B = _mm_malloc(order * order * sizeof(float), ALIGN);
    cblas_scopy(order * order, A, 1, B, 1);
    transpose_matrix(B, order);
    float l1_norm, max_norm;
    get_matrix_norms(A, order, &l1_norm, &max_norm);
    cblas_sscal(order * order, 1.0 / (l1_norm * max_norm), B, 1);
    float* R = get_identity_matrix(order);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, -1, B, order, A, order, 1, R, order);
    float* R_1_deg = _mm_malloc(order * order * sizeof(float), ALIGN);
    cblas_scopy(order * order, R, 1, R_1_deg, 1);
    float* tmp = NULL;
    float* inv_A = get_identity_matrix(order);
    for(int i = 0; i < iter_number; ++i){
        tmp = _mm_malloc(order * order * sizeof(float), ALIGN);
        cblas_saxpy(order * order, 1, R, 1, inv_A, 1);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, 1, R, order, R_1_deg, order, 0, tmp,
                    order);
        _mm_free(R);
        R = tmp;
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, 1, inv_A, order, B, order, 0.0, tmp,
            order);
    _mm_free(inv_A);
    inv_A = tmp;
    _mm_free(B);
    _mm_free(R_1_deg);
    return inv_A;
}

int main(){
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int N = 0, M = 0;
    fscanf(input, "%d%d", &N, &M);
    float* A = gen_matrix(N, 5.0);
    time_t start_real = time(NULL);
    float* inv_A = invert_matrix(A, N, M);
    time_t finish_real = time(NULL);
    float* rez = _mm_malloc(N * N * sizeof(float), ALIGN);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1, inv_A, N, A, N, 0, rez, N);
    print_matrix(rez, N, output);
    printf("Total real time: %ld sec.\n", finish_real - start_real);
    _mm_free(A);
    _mm_free(inv_A);
    _mm_free(rez);
    fclose(input);
    fclose(output);
    return 0;
}