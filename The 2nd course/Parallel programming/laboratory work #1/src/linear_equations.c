#include "linear_equations.h"

struct matrix* solve_system(struct matrix* A, struct matrix* b, int rank, float eps, float t,
        int* recvcounts, int * displs) {
    int first = displs[rank] / (b->cols_ + b->cols_align_);
    int last = first + recvcounts[rank] / (b->cols_ + b->cols_align_);
    struct matrix* x = create_matrix(b->rows_, b->cols_);
    float global_norm = 0;
    do {
        struct matrix* Ax = create_matrix(b->rows_, b->cols_);
        global_norm = 0;
        mul_matrices(A, x, Ax, first);  //A*x
        sub_matrices(Ax, b, first, last);   //A*x - b
        mul_matrix_on_scalar(Ax, t);        //t(A*x - b)
        sub_matrices(x, Ax, first, last);   //x(n) - t(A*x - b)
        free_matrix(Ax);
        Ax = create_matrix(b->rows_, b->cols_);
        MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_FLOAT, x->matrix_, recvcounts, displs, MPI_FLOAT, MPI_COMM_WORLD);
        mul_matrices(A, x, Ax, first);  //A*x(n+1)
        sub_matrices(Ax, b, first, last);   //A*x(n+1) - b
        float local_norm = squared_euclid_norm(Ax); // ||A*x(n+1) - b||
        MPI_Allreduce(&local_norm, &global_norm, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
        free_matrix(Ax);
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

struct matrix* gen_system(struct matrix* A, struct matrix* b, int first) {
    for(int i = 0; i < A->rows_; i++) {
        for(int j = 0; j < A->cols_; j++) {
            *get_element(A, i, j) = (i + first == j) ? 2.0 : 1.0;
        }
    }

    struct matrix* u = create_matrix(b->rows_, b->cols_);

    for(int i = 0; i < u->rows_; i++) {
        *get_element(u, i, 0) = sin((2 * PI * i) / u->rows_);
    }

    mul_matrices(A, u, b, first);
    return u;
}