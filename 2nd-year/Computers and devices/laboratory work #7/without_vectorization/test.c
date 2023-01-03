#include "matrix.h"

int main(){
    FILE* input = fopen("input.txt", "r");
    FILE* output = fopen("output.txt", "w");
    int N = 0, M = 0;
    fscanf(input, "%d%d", &N, &M);
    struct matrix* A = gen_matrix(N, 5.0);
    time_t start_real = time(NULL);
    struct matrix* inv_A = invert_matrix(A, M);
    time_t finish_real = time(NULL);
    struct matrix* rez = mul_matrices(inv_A, A);    //test an answer by multiplication it on the source matrix
    print_matrix(rez, output);
    printf("Total real time: %ld sec.\n", finish_real - start_real);
    free_matrix(A);
    free_matrix(inv_A);
    free_matrix(rez);
    fclose(input);
    fclose(output);
    return 0;
}