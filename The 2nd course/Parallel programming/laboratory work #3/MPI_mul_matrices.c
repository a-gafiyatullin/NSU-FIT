//
// Created by xp10rd on 4/10/19.
//

#include "MPI_mul_matrices.h"

void create_topology(int **dims, MPI_Comm  *comm_2D, MPI_Comm *comm_X, MPI_Comm *comm_Y) {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    /* create a new 2D topology */
    *dims = (int*)calloc(DIMS, sizeof(int));
    int *periods = (int*)calloc(DIMS, sizeof(int));
    free(periods);
    MPI_Dims_create(size, DIMS, *dims);  //automatic topology creation
    MPI_Cart_create(MPI_COMM_WORLD, DIMS, *dims, periods, TRUE, comm_2D);

    /* create sub cart of the topology */
    int *remain_dims_y = (int*)malloc(DIMS * sizeof(int));
    remain_dims_y[0] = FALSE;
    remain_dims_y[1] = TRUE;
    int *remain_dims_x = (int*)malloc(DIMS * sizeof(int));
    remain_dims_x[0] = TRUE;
    remain_dims_x[1] = FALSE;
    MPI_Cart_sub(*comm_2D, remain_dims_x, comm_X);
    MPI_Cart_sub(*comm_2D, remain_dims_y, comm_Y);
    free(remain_dims_x);
    free(remain_dims_y);
}

/* get information about parts of A and B matrices in the each process */
void bcast_matrices_dimensions(int root_rank, int *A_rows, int *A_cols, int *B_cols, MPI_Comm comm) {
    MPI_Bcast(A_cols, 1, MPI_INT, root_rank, comm);
    MPI_Bcast(A_rows, 1, MPI_INT, root_rank, comm);
    MPI_Bcast(B_cols, 1, MPI_INT, root_rank, comm);
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

/* create new type for interprocess communications */
void create_ipc_type(int rows, int cols, int stride, MPI_Aint extent, MPI_Datatype *sub_matrix) {
    MPI_Datatype vert_split;
    MPI_Type_vector(rows, cols, stride, MPI_DOUBLE, &vert_split);
    MPI_Type_create_resized(vert_split, (MPI_Aint)0, extent, sub_matrix);
    MPI_Type_commit(sub_matrix);
}

struct matrix *MPI_mul_matrices(struct matrix *A, struct matrix *B) {
    int *dims, coords[DIMS] = {0}, rank, global_root_rank;
    int A_rows, A_cols, B_cols;
    MPI_Comm comm_2D, comm_X, comm_Y;
    create_topology(&dims, &comm_2D, &comm_X, &comm_Y);

    /* information for MPI_Scatterv */
    int sendcount_A[dims[X]];
    int displs_A[dims[X]];
    int sendcount_B[dims[Y]];
    int displs_B[dims[Y]];

    /* each process gets information about himself */
    MPI_Cart_rank(comm_2D, coords, &global_root_rank);
    MPI_Comm_rank(comm_2D, &rank);
    MPI_Cart_coords(comm_2D, rank, DIMS, coords);
    if(rank == global_root_rank) {
        printf("X = %d, Y = %d\n", dims[X], dims[Y]);
        A_rows = A->rows_;
        A_cols = A->cols_;
        B_cols = B->cols_;
    }
    bcast_matrices_dimensions(global_root_rank, &A_rows, &A_cols, &B_cols, comm_2D);

    gen_scatterv_info(A_rows, A_cols, dims[X], sendcount_A, displs_A);
    gen_scatterv_info(B_cols, A_cols, dims[Y], sendcount_B, displs_B);

    /* create the parts of the matrices in the each process */
    struct matrix *part_A = create_matrix(sendcount_A[coords[X]] / A_cols, A_cols);
    struct matrix *part_B = create_matrix(A_cols, sendcount_B[coords[Y]] / A_cols);

    MPI_Datatype sub_B;
    create_ipc_type(A_cols, sendcount_B[coords[Y]] / A_cols, B_cols,
            (MPI_Aint)(sendcount_B[coords[Y]] / A_cols * sizeof(double)), &sub_B);

    if(coords[X] == 0 || coords[Y] == 0) {
        int blocklength[dims[Y]];
        int displs_sub_B[dims[Y]];
        for(int i = 0; i < dims[Y]; i++) {
            displs_sub_B[i] = i;
            blocklength[i] = 1;
        }
        /* send part of the A matrix in X dimension */
        if(coords[Y] == 0) {
            MPI_Scatterv(A->matrix_, sendcount_A, displs_A, MPI_DOUBLE, part_A->matrix_, sendcount_A[coords[X]],
                         MPI_DOUBLE, global_root_rank, comm_X);
        }
        /* send part of the B matrix in Y dimension */
        if(coords[X] == 0) {
            MPI_Scatterv(B->matrix_, blocklength, displs_sub_B, sub_B, part_B->matrix_, sendcount_B[coords[Y]],
                         MPI_DOUBLE, global_root_rank, comm_Y);
        }
    }

    /* send part of the matrices to other processes */
    MPI_Bcast(part_A->matrix_, sendcount_A[coords[X]], MPI_DOUBLE, 0, comm_Y);
    MPI_Bcast(part_B->matrix_, sendcount_B[coords[Y]], MPI_DOUBLE, 0, comm_X);

    /* multiplication of the matrices parts */
    struct matrix *rezult = mul_matrices(part_A, part_B);

    /* get all parts of the result in global root process */
    int dispC[dims[X] * dims[Y]];
    int countC[dims[X] * dims[Y]];
    for(int i = 0; i < dims[X]; i++) {
        for(int j = 0; j < dims[Y]; j++) {
            dispC[i * dims[Y] + j] = i * dims[Y] * rezult->rows_ + j;
            countC[i * dims[Y] + j] = 1;
        }
    }
    struct matrix *C;
    MPI_Datatype sub_C;
    create_ipc_type(rezult->rows_, rezult->cols_, B_cols, (MPI_Aint)(rezult->cols_ * sizeof(double)), &sub_C);
    if(rank == global_root_rank) {
        C = create_matrix(A_rows, B_cols);
    }
    MPI_Gatherv(rezult->matrix_, rezult->cols_ * rezult->rows_, MPI_DOUBLE, C->matrix_, countC, dispC,
                sub_C, global_root_rank, comm_2D);

    free_matrix(part_A);
    free_matrix(part_B);
    free(dims);
    MPI_Comm_free(&comm_2D);
    MPI_Comm_free(&comm_X);
    MPI_Comm_free(&comm_Y);
    MPI_Type_free(&sub_B);
    MPI_Type_free(&sub_C);
    return C;
}