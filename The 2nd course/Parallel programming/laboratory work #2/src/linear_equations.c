#include "linear_equations.h"

struct matrix* solve_system(struct matrix* A, struct matrix* b, float eps, float t) {
    struct matrix* x = create_matrix(b->rows_, b->cols_);
    float b_norm = sqrtf(squared_euclid_norm(b));
    struct matrix* Ax = create_matrix(b->rows_, b->cols_);
    float norm;
    mul_matrices(A, x, Ax);  //A*x
    sub_matrices(Ax, b);   //A*x - b
    do {
        mul_matrix_on_scalar(Ax, t);        //t(A*x - b)
        sub_matrices(x, Ax);   //x(n) - t(A*x - b)
        mul_matrices(A, x, Ax);  //A*x(n+1)
        sub_matrices(Ax, b);   //A*x(n+1) - b
        norm = squared_euclid_norm(Ax); // ||A*x(n+1) - b||
    } while (sqrtf(norm) / b_norm > eps);
    free_matrix(Ax);
    return x;
}

struct matrix* gen_system(struct matrix* A, struct matrix* b) {
    for(int i = 0; i < A->rows_; i++) {
        for(int j = 0; j < A->cols_; j++) {
            *get_element(A, i, j) = (i == j) ? 2.0f : 1.0f;
        }
    }

    struct matrix* u = create_matrix(b->rows_, b->cols_);

    for(int i = 0; i < u->rows_; i++) {
        *get_element(u, i, 0) = sinf((2 * PI * i) / u->rows_);
    }

    mul_matrices(A, u, b);
    return u;
}