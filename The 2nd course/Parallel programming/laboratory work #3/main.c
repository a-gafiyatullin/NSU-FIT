#include <stdio.h>
#include "MPI_mul_matrices.h"
#include "mpi.h"

int main(int argc, char *argv[]) {
    /* initialize environment */
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* get A and B matrices dimensions in root process */
    struct matrix *A, *B;
    if(rank == 0) {
        int A_rows, A_cols, B_cols;
        fscanf(stdin, "%d%d%d", &A_rows, &A_cols, &B_cols);
        //A = create_matrix(A_rows, A_cols);
        //B = create_matrix(A_cols, B_cols);
        //get_matrix(A, stdin);   /* get part of the A and B matrices in the each process */
        //get_matrix(B, stdin);
        A = gen_matrix(A_rows, A_cols);
        B = gen_matrix(A_cols, B_cols);
        print_matrix(A, stdout);
        print_matrix(B, stdout);
    }
    struct matrix *C = MPI_mul_matrices(A, B);
    if(rank == 0) {
        print_matrix(C, stdout);
    }

    MPI_Finalize();
    return 0;
}