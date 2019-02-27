#include "linear_equations.h"
#define EPS 1e-5 //accuracy
#define T 1e-2  //parameter
#define DIMM 1200

int main(int argc, char* argv[]) {
    int comm_size, rank;
    double start, finish;
    struct matrix* b = create_matrix(DIMM, 1);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* recvcounts, *displs;
    get_borders(b, comm_size, &recvcounts, &displs);
    int first = displs[rank] / (b->cols_ + b->cols_align_); // get first and last processed row of this process
    int last = first + recvcounts[rank] / (b->cols_ + b->cols_align_);
    struct matrix* A = create_matrix(last - first, DIMM);
    gen_system(A, b, first);    //generate part of the matrix for each process

    start = MPI_Wtime();
    struct matrix* x = solve_system(A, b, EPS, T, );
    //MPI_Allgatherv(x->matrix_, recvcounts[rank], MPI_FLOAT, ans->matrix_, recvcounts, displs, MPI_FLOAT, MPI_COMM_WORLD);
    finish= MPI_Wtime();
    if(rank == 0) {
        printf("Answer: \n");
        print_matrix(x, stdout);
        fprintf(stdout, "Time: %lf sec.\n", finish - start);
    }
    free_matrix(A);
    free_matrix(b);
    free_matrix(x);
    MPI_Finalize();
    return 0;
}