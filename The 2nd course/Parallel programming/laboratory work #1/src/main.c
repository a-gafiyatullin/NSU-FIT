#include "linear_equations.h"
#define EPS 1e-3 //accuracy
#define T 1e-4  //parameter
#define DIMM 1200

int main(int argc, char* argv[]) {
    int comm_size, rank;
    double start, finish;
    struct matrix* A = create_matrix(DIMM, DIMM);
    struct matrix* b = create_matrix(DIMM, 1);
    struct matrix* u;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) { /* process № 0 get matrices */
        u = gen_system(A, b);
        fprintf(stdout, "Right answer: \n");
        print_matrix(u, stdout);
    }
    MPI_Bcast(A->matrix_, size_matrix(A), MPI_FLOAT, 0, MPI_COMM_WORLD);    /* send the matrices to each process */
    MPI_Bcast(b->matrix_, size_matrix(b), MPI_FLOAT, 0, MPI_COMM_WORLD);
    start = MPI_Wtime();
    struct matrix* x = solve_system(A, b, comm_size, rank, EPS, T);
    finish= MPI_Wtime();
    if(rank == 0) {
        printf("Real answer: \n");
        print_matrix(x, stdout);
        fprintf(stdout, "Time: %lf sec.\n", finish - start);
    }
    free_matrix(A);
    free_matrix(b);
    free_matrix(x);
    free_matrix(u);
    MPI_Finalize();
    return 0;
}