#include "linear_equations.h"
#define EPS 1e-4 //accuracy
#define T 1e-3  //parameter
#define DIMM 1400

int main(int argc, char* argv[]) {
    int comm_size, rank;
    double start, finish;
    struct matrix* u;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* recvcounts, *displs;
    get_borders(DIMM, comm_size, &recvcounts, &displs);
    int first = displs[rank]; //get first and last processed row of this process
    int last = first + recvcounts[rank];
    struct matrix* A = create_matrix(last - first, DIMM);
    struct matrix* b = create_matrix(last - first, 1);
    u = gen_system(A, b, first);    //generate part of the matrix for each process
    if(rank == 0) {
        printf("Right answer:\n");
        print_matrix(u, stdout);
    }

    start = MPI_Wtime();
    struct matrix* x = solve_system(A, b, rank, comm_size, EPS, T, recvcounts, displs);
    finish = MPI_Wtime();

    //each process prints his part of the answer
    int turn = 0;
    if(comm_size != 1) {
        if (rank == 0) {
            printf("\nReal answer:\n");
            print_matrix(x, stdout);
            MPI_Send(&turn, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        } else if (rank != comm_size - 1) {
            MPI_Recv(&turn, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_matrix(x, stdout);
            MPI_Send(&turn, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&turn, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_matrix(x, stdout);
            printf("\nTime: %f\n", finish - start);
        }
    }
    else {
        printf("\nReal answer:\n");
        print_matrix(x, stdout);
        printf("\nTime: %f\n", finish - start);
    }
    free_matrix(A);
    free_matrix(b);
    free_matrix(x);
    free_matrix(u);
    free(recvcounts);
    free(displs);
    MPI_Finalize();
    return 0;
}