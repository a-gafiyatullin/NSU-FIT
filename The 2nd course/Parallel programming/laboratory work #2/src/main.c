#include "linear_equations.h"
#define EPS 1e-3 //accuracy
#define T 1e-4  //parameter
#define NUM_THREADS_ARG 1
#define DIMM 2500

int main(int argc, char* argv[]) {
    double start, finish;
    struct matrix* b = create_matrix(DIMM, 1);
    struct matrix* u;
    struct matrix* A = create_matrix(DIMM, DIMM);
    u = gen_system(A, b);
    printf("Right answer:\n");
    print_matrix(u, stdout);

    omp_set_num_threads(atoi(argv[NUM_THREADS_ARG]));
    start = omp_get_wtime();
    struct matrix* x = solve_system(A, b, EPS, T);
    finish = omp_get_wtime();

    printf("Real answer:\n");
    print_matrix(x, stdout);
    fprintf(stdout, "Time: %lf sec.\n", finish - start);

    free_matrix(A);
    free_matrix(b);
    free_matrix(x);
    free_matrix(u);
    return 0;
}