#include "linear_equations.h"

struct matrix* solve_system(struct matrix* A, struct matrix* b, float eps, float t, int first, int last) {
    struct matrix* x = create_matrix(b->rows_, b->cols_);
    struct matrix* sub_b = get_submatrix(b, first, last);
    struct matrix* Ax = NULL;
    float global_norm;
    do {
        global_norm = 0;
        free_matrix(Ax);
        Ax = mul_matrices(A, x);
        sub_matrices(Ax, sub_b);
        mul_matrix_on_scalar(Ax, t);
        sub_matrices(x, Ax);
        free_matrix(Ax);
        Ax = mul_matrices(A, x);
        sub_matrices(Ax, sub_b);
        float local_norm = squared_euclid_norm(Ax);
        MPI_Allreduce(&local_norm, &global_norm, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    } while(sqrt(global_norm) / sqrt(squared_euclid_norm(b)) > eps);
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

void gen_system(struct matrix* A, struct matrix* b, int first) {
    for(int i = 0; i < A->rows_; i++) {
        for(int j = 0; j < A->cols_; j++) {
            *get_element(A, i, j) = (i + first == j) ? 2.0 : 1.0;
        }
    }

    for(int i = 0; i < b->rows_; i++) {
        *get_element(b, i, 0) = b->rows_ + 1;
    }
}