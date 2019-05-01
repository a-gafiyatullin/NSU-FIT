#include <stdio.h>
#include "MPI_mul_matrices.h"

int main(int argc, char *argv[]) {
    /* initialize environment */
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* get A and B matrices dimensions in root process */
    struct matrix *A, *B;
    if(rank == 0) {
        MPI_create_task(&A, &B, atoi(argv[1]));
        print_matrix(A, stdout);
        print_matrix(B, stdout);
    }
    double start = MPI_Wtime();
    struct matrix *C = MPI_mul_matrices(A, B);
    double end = MPI_Wtime();
    if(rank == 0) {
        print_matrix(C, stdout);
        printf("Total process time: %lf\n", end - start);
    }

    MPI_Finalize();
    return 0;
}