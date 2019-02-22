#include "linear_equations.h"

struct matrix* solve_system(struct matrix* A, struct matrix* b, int comm_size, int rank, float eps, float t) {
    int* recvcounts, *displs;
    get_borders(b, comm_size, &recvcounts, &displs);
    int first = displs[rank] / (b->cols_ + b->cols_align_); // get first and last processed row of this process
    int last = first + recvcounts[rank] / (b->cols_ + b->cols_align_);
    struct matrix* x = create_matrix(b->rows_, b->cols_);
    struct matrix* Ax = NULL;
    do {
        free_matrix(Ax);
        Ax = mul_matrices(A, x, first, last);
        sub_matrices(Ax, b);
        mul_matrix_on_scalar(Ax, t);
        sub_matrices(x, Ax);
        free_matrix(Ax);
        MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_FLOAT, x->matrix_, recvcounts, displs,
                MPI_FLOAT, MPI_COMM_WORLD);
        Ax = mul_matrices(A, x, 0, A->rows_);
        sub_matrices(Ax, b);
    } while(euclid_norm(Ax) / euclid_norm(b) > eps);

    return x;
}

void get_borders(struct matrix* m, int comm_size, int** recvcounts, int** displs) {
    *recvcounts = malloc(sizeof(int) * comm_size);
    *displs = malloc(sizeof(int) * comm_size);
    int r = m->rows_ % comm_size;   //amount of the unprocessed rows
    int rows_per_pr = m->rows_ / comm_size; //rows per process
    for(int i = 0; i < comm_size; i++) { //distributing an unprocessed rows
        (*displs)[i] = get_element(m, rows_per_pr * i + (i < r ? i : r), 0) - get_element(m, 0, 0);
        (*recvcounts)[i] = get_element(m, rows_per_pr * (i + 1) + (i < r ? i + 1: r), 0)
                - get_element(m, rows_per_pr * i + (i < r ? i : r), 0);
    }
}

struct matrix* gen_system(struct matrix* A, struct matrix* b) {
    for(int i = 0; i < A->rows_; i++) {
        for(int j = 0; j < A->cols_; j++) {
            *get_element(A, i, j) = (i == j) ? 2.0 : 1.0;
        }
    }

    struct matrix* u = create_matrix(b->rows_, b->cols_);

    for(int i = 0; i < u->rows_; i++) {
        *get_element(u, i, 0) = sin((2 * PI * i) / u->rows_);
    }

    struct matrix* tmp = mul_matrices(A, u, 0, A->rows_);
    copy_matrix(b, tmp);
    return u;
}