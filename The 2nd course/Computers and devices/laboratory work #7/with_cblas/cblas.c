#include <stdio.h>  //printf
#include <stdlib.h> //malloc, calloc, free
#include <string.h> //memset
#include <time.h>   //time
#include <cblas.h>  //cblas_scopy, cblas_sscal, cblas_sgemm
#include <sys/times.h>  //times
#include <unistd.h> //sysconf
#include <math.h>   //fabsf

float* gen_matrix(int order, float range){
    float* m = malloc(order * order * sizeof(float));
    srand(time(NULL));
    for(int i = 0; i < order * order; i++)
        m[i] = ((float)rand() / (float)(RAND_MAX)) * range;
    return m;
}

void transpose_matrix(float* m, int order){
    for(int i = 0; i < order; i++)
        for(int j = i; j < order; j++){
            float tmp = m[i * order + j];
            m[i * order + j] = m[j * order + i];
            m[j * order + i] = tmp;
        }
}

void print_matrix(float* m, int order, FILE* out){
    fprintf(out, "Order: %d\n", order);
    for(int i = 0; i < order; i++){
        for(int j = 0; j < order; j++)
            fprintf(out, "%.4f ", m[i * order + j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void get_matrix_norms(float* m, int order, float* l1_norm, float* max_norm){
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < order; i++){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < order; j++){
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
    float* m = malloc(order * order * sizeof(float));
    memset(m, 0, order * order * sizeof(float));
    for(int i = 0; i < order; i++)
        m[i * order + i] = 1;
    return m;
}

float* invert_matrix(float* A, int order, int iter_number){
    float* B = malloc(order * order * sizeof(float));
    cblas_scopy(order * order, A, 1, B, 1);
    transpose_matrix(B, order);
    float l1_norm, max_norm;
    get_matrix_norms(A, order, &l1_norm, &max_norm);
    cblas_sscal(order * order, 1.0 / (l1_norm * max_norm), B, 1);
    float* R = get_identity_matrix(order);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, -1, B, order, A, order, 1, R, order);
    float* R_1_deg = malloc(order * order * sizeof(float));
    cblas_scopy(order * order, R, 1, R_1_deg, 1);
    float* tmp = NULL;
    float* inv_A = get_identity_matrix(order);
    for(int i = 0; i < iter_number; i++){
        tmp = malloc(order * order * sizeof(float));
        cblas_saxpy(order * order, 1, R, 1, inv_A, 1);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, 1, R, order, R_1_deg, order, 0, tmp,
                    order);
        free(R);
        R = tmp;
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, order, order, order, 1, inv_A, order, B, order, 0.0, tmp,
            order);
    free(inv_A);
    inv_A = tmp;
    free(B);
    free(R_1_deg);
    return inv_A;
}

int main(){
    struct tms start, finish;
    long long int clocks_per_sec = sysconf(_SC_CLK_TCK);
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int N = 0, M = 0;
    fscanf(input, "%d%d", &N, &M);
    float* A = gen_matrix(N, 5.0);
    time_t start_real = time(NULL);
    times(&start);
    float* inv_A = invert_matrix(A, N, M);
    times(&finish);
    time_t finish_real = time(NULL);
    double total_process_time = finish.tms_utime - start.tms_utime;
    float* rez = malloc(N * N * sizeof(float));
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1, inv_A, N, A, N, 0, rez, N);
    print_matrix(rez, N, output);
    printf("Total process time: %lf sec.\n", total_process_time / clocks_per_sec);
    printf("Total real time: %ld sec.\n", finish_real - start_real);
    free(A);
    free(inv_A);
    free(rez);
    fclose(input);
    fclose(output);
    return 0;
}