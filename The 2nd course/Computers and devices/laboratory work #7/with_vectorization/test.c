#include "matrix.h"

int main(){
    struct tms start, finish;
    long long int clocks_per_sec = sysconf(_SC_CLK_TCK);
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int N = 0, M = 0;
    fscanf(input, "%d%d", &N, &M);
    struct matrix* A = gen_matrix(N, 5.0);
    time_t start_real = time(NULL);
    times(&start);
    struct matrix* inv_A = invert_matrix(A, M);
    times(&finish);
    time_t finish_real = time(NULL);
    double total_process_time = finish.tms_utime - start.tms_utime;
    struct matrix* rez = mul_matrices(inv_A, A);
    print_matrix(rez, output);
    printf("Total process time: %lf sec.\n", total_process_time / clocks_per_sec);
    printf("Total real time: %ld sec.\n", finish_real - start_real);
    free_matrix(A);
    free_matrix(inv_A);
    free_matrix(rez);
    fclose(input);
    fclose(output);
    return 0;
}