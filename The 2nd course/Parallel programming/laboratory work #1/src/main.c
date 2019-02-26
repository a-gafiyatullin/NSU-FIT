#include "linear_equations.h"
#define EPS 1e-3 //accuracy
#define T 1e-4  //parameter
#define DIMM 1200

int main(int argc, char* argv[]) {
    int comm_size, rank;
    double start, finish;
    struct matrix* b = create_matrix(DIMM, 1);
    struct matrix* u;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* recvcounts, *displs;
    get_borders(b, comm_size, &recvcounts, &displs);
    int first = displs[rank] / (b->cols_ + b->cols_align_); // get first and last processed row of this process
    int last = first + recvcounts[rank] / (b->cols_ + b->cols_align_);
    struct matrix* A = create_matrix(last - first, DIMM);
    u = gen_system(A, b, first);    //generate part of the matrix for each process
    MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_FLOAT, b->matrix_, recvcounts, displs, MPI_FLOAT, MPI_COMM_WORLD);
    if(rank == 0) {
        printf("Right answer:\n");
        print_matrix(u, stdout);
    }
    start = MPI_Wtime();
    struct matrix* x = solve_system(A, b, rank, EPS, T, recvcounts, displs);
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