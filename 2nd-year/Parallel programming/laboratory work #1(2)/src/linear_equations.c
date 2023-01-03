#include "linear_equations.h"

void distr_mul_matrices(struct matrix* A, struct matrix** x, struct matrix* Ax, int rank, int comm_size,
        int* recvcounts, int* displs);

struct matrix* solve_system(struct matrix* A, struct matrix* b, int rank, int comm_size, float eps, float t,
        int* recvcounts, int * displs) {
    struct matrix* x = create_matrix(b->rows_, b->cols_);
    float local_b_norm = squared_euclid_norm(b);
    float global_b_norm = 0;
    MPI_Allreduce(&local_b_norm, &global_b_norm, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    struct matrix* Ax = create_matrix(b->rows_, b->cols_);
    distr_mul_matrices(A, &x, Ax, rank, comm_size, recvcounts, displs);  //A*x
    sub_matrices(Ax, b);    //A*x - b
    float global_norm = 0;
    do {
        global_norm = 0;
        mul_matrix_on_scalar(Ax, t);    //t(A*x - b)
        sub_matrices(x, Ax);   //x(n) - t(A*x - b)
        free_matrix(Ax);
        Ax = create_matrix(b->rows_, b->cols_);
        distr_mul_matrices(A, &x, Ax, rank, comm_size, recvcounts, displs); //A*x(n+1)
        sub_matrices(Ax, b);   //A*x(n+1) - b
        float local_norm = squared_euclid_norm(Ax); // ||A*x(n+1) - b||
        MPI_Allreduce(&local_norm, &global_norm, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    } while(sqrt(global_norm) / sqrt(global_b_norm) > eps);

    free_matrix(Ax);
    return x;
}

//multiply A by distributed x and save answer to Ax
void distr_mul_matrices(struct matrix* A, struct matrix** x, struct matrix* Ax, int rank, int comm_size,
        int* recvcounts, int* displs) {
    struct matrix* recv = transpose_matrix(*x);
    int cols = (*x)->cols_;
    for(int i = 0; i < comm_size; i++) {
        mul_matrices_tr_b(A, recv, Ax, displs[(rank - i + comm_size) % comm_size]);
        struct matrix* send = non_init_create_matrix(recv->rows_, recv->cols_);
        copy_matrix(send, recv);
        free_matrix(recv);
        recv = create_matrix(cols, recvcounts[(rank - i - 1 + comm_size) % comm_size]);
        MPI_Sendrecv(send->matrix_, size_matrix(send), MPI_FLOAT, (rank + 1) % comm_size, (rank + 1) % comm_size,
                     recv->matrix_, size_matrix(recv), MPI_FLOAT, (rank - 1 + comm_size) % comm_size, rank,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        free_matrix(send);
    }
    *x = transpose_matrix(recv);
    free_matrix(recv);
}

void get_borders(int dimm, int comm_size, int** recvcounts, int** displs) {
    *recvcounts = malloc(sizeof(int) * comm_size);
    *displs = malloc(sizeof(int) * comm_size);
    int r = dimm % comm_size;   //amount of the unprocessed rows
    int rows_per_pr = dimm / comm_size; //rows per process
    for(int i = 0; i < comm_size; i++) { //distributing an unprocessed rows
        (*displs)[i] = rows_per_pr * i + (i < r ? i : r);
        (*recvcounts)[i] = rows_per_pr + (i < r ? 1 : 0);
    }
}

struct matrix* gen_system(struct matrix* A, struct matrix* b, int first) {
    for(int i = 0; i < A->rows_; i++) {
        for(int j = 0; j < A->cols_; j++) {
            *get_element(A, i, j) = (i + first == j) ? 2.0 : 1.0;
        }
    }

    struct matrix* u = create_matrix(A->cols_, 1);

    for(int i = 0; i < u->rows_; i++) {
        *get_element(u, i, 0) = sin((2 * PI * i) / u->rows_);
    }

    mul_matrices(A, u, b, 0);
    return u;
}