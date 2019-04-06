#include <stdio.h>
#include "matrix.h"
#include "mpi.h"

#define DIMS 2
#define TRUE 1
#define FALSE 0
#define X 0
#define Y 1

void gen_scatterv_info(int rows, int cols, int size, int *sendcount, int *displs);

int main(int argc, char *argv[]) {
    /* initialize environment */
    int size, coords[DIMS] = {0}, rank, root_rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* create a new 2D topology */
    int dims[DIMS] = {0};
    int periods[DIMS] = {0};
    MPI_Dims_create(size, DIMS, dims);  //automatic topology creation
    MPI_Comm comm_2d;
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, dims, periods, TRUE, &comm_2d);

    /* create sub cart of the topology */
    MPI_Comm comm_x, comm_y;
    int remain_dims_y[DIMS] = {FALSE, TRUE};
    int remain_dims_x[DIMS] = {TRUE, FALSE};
    MPI_Cart_sub(comm_2d, remain_dims_x, &comm_x);
    MPI_Cart_sub(comm_2d, remain_dims_y, &comm_y);

    /* information for MPI_Scatterv */
    int sendcount_A[dims[X]];
    int displs_A[dims[X]];
    int sendcount_B[dims[Y]];
    int displs_B[dims[Y]];

    /* get A and B matrices dimensions in root process */
    int A_rows, A_cols, B_cols;
    struct matrix *part_A;
    struct matrix *part_B;
    MPI_Cart_rank(comm_2d, coords, &root_rank);
    MPI_Comm_rank(comm_2d, &rank);
    MPI_Cart_coords(comm_2d, rank, DIMS, coords);
    if(rank == root_rank) {
        fscanf(stdin, "%d%d%d", &A_rows, &A_cols, &B_cols);
    }

    /* get information about parts of A and B matrices in the each process */
    MPI_Bcast(&A_cols, 1, MPI_INT, root_rank, comm_2d);
    MPI_Bcast(&A_rows, 1, MPI_INT, root_rank, comm_2d);
    MPI_Bcast(&B_cols, 1, MPI_INT, root_rank, comm_2d);
    gen_scatterv_info(A_rows, A_cols, dims[X], sendcount_A, displs_A);
    gen_scatterv_info(B_cols, A_cols, dims[Y], sendcount_B, displs_B);

    /* create the parts of the matrices in the each process */
    part_A = create_matrix(sendcount_A[coords[X]] / A_cols, A_cols);
    part_B = create_matrix(A_cols, sendcount_B[coords[Y]] / A_cols);

    /* get part of the A and B matrices in the each process */
    if(rank == root_rank) {
        /* get matrices */
        struct matrix *A, *B;
        A = create_matrix(A_rows, A_cols);
        B = create_matrix(A_cols, B_cols);
        get_matrix(A, stdin);
        get_matrix(B, stdin);

        /* create new type for interprocess communications */
        MPI_Datatype vert_split, comm_type;
        MPI_Type_vector(A_cols, sendcount_B[0] / A_cols, B_cols, MPI_DOUBLE, &vert_split);
        MPI_Type_create_resized(vert_split, (MPI_Aint)0, (MPI_Aint)(sendcount_B[0] / A_cols * sizeof(double)),
                                &comm_type);
        MPI_Type_commit(&comm_type);

        int blocklength[dims[Y]];
        int displs_comm_type[dims[Y]];
        for(int i = 0; i < dims[Y]; i++) {
            displs_comm_type[i] = i;
            blocklength[i] = 1;
        }
        /* send part of the A matrix in X dimension */
        MPI_Scatterv(A->matrix_, sendcount_A, displs_A, MPI_DOUBLE, part_A->matrix_, sendcount_A[coords[X]], MPI_DOUBLE,
                root_rank, comm_x);
        /* send part of the B matrix in Y dimension */
        MPI_Scatterv(B->matrix_, blocklength, displs_comm_type, comm_type, part_B->matrix_, sendcount_B[coords[Y]],
                MPI_DOUBLE, root_rank, comm_y);
        free_matrix(A);
        free_matrix(B);
    } else {
        double dummy;   //because sendbuf cannot be MPI_IN_PLACE
        /* get part of the A matrix in X dimension for processes with Y = 0 */
        if(coords[Y] == 0) {
            MPI_Scatterv(&dummy, sendcount_A, displs_A, MPI_DOUBLE, part_A->matrix_, sendcount_A[coords[X]], MPI_DOUBLE,
                         root_rank, comm_x);
        }
        /* send part of the B matrix in Y dimension for processes with X = 0 */
        if(coords[X] == 0) {
            MPI_Scatterv(&dummy, sendcount_B, displs_B, MPI_DOUBLE, part_B->matrix_, sendcount_B[coords[Y]], MPI_DOUBLE,
                    root_rank, comm_y);
        }
    }

    /* send part of the matrices to other processes */
    MPI_Bcast(part_A->matrix_, sendcount_A[coords[X]], MPI_DOUBLE, 0, comm_y);
    MPI_Bcast(part_B->matrix_, sendcount_B[coords[Y]], MPI_DOUBLE, 0, comm_x);

//    if(coords[X] == 2 && coords[Y] == 2) {
//        print_matrix(part_A, stdout);
//        print_matrix(part_B, stdout);
//    }

    free_matrix(part_A);
    free_matrix(part_B);
    MPI_Finalize();
    return 0;
}

void gen_scatterv_info(int rows, int cols, int size, int *sendcount, int *displs) {
    int rows_per_proc = rows / size;
    int remainder = rows % size;
    displs[0] = 0;
    sendcount[0] = (rows_per_proc + (0 < remainder ? 1 : 0)) * cols;
    for(int i = 1; i < size; i++) {
        displs[i] = displs[i - 1] + sendcount[i - 1];
        sendcount[i] = (rows_per_proc + (i < remainder ? 1 : 0)) * cols;
    }
}