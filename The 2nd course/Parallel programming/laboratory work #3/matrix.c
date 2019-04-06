//
// Created by xp10rd on 4/1/19.
//

#include "matrix.h"

struct matrix* create_matrix(int rows, int cols) {
    struct matrix *m = malloc(sizeof(struct matrix));
    m->rows_ = rows;
    m->cols_ = cols;
    m->matrix_ = malloc(sizeof(double) * rows * cols);
    return m;
}

void get_matrix(struct matrix *m, FILE *in){
    register int order = m->rows_ * m->cols_;
    for(int i = 0; i < order; ++i)
        fscanf(in, "%lf", &m->matrix_[i]);
}

void print_matrix(struct matrix *m, FILE *out) {
    for(int i = 0; i < m->rows_; ++i){
        for(int j = 0; j <  m->cols_; ++j)
            fprintf(out, "%.4lf ", m->matrix_[i * m->cols_ + j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void free_matrix(struct matrix *m) {
    free(m->matrix_);
    free(m);
}

struct matrix *transpose_matrix(struct matrix* m){
    struct matrix* tr_m = create_matrix(m->cols_, m->rows_);
    register int rows = m->rows_;
    register int cols = m->cols_;
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            tr_m->matrix_[j * rows + i] = m->matrix_[i * cols + j];
        }
    }
    return tr_m;
}

struct matrix *mul_matrices(struct matrix *a, struct matrix *b) {
    register int rows = a->rows_;
    register int cols = b->cols_;
    register int order = a->cols_;
    struct matrix* tmp = create_matrix(rows, cols);
    for(int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j) {
            tmp->matrix_[i * cols + j] = 0;
            for (int k = 0; k < order; ++k)
                tmp->matrix_[i * cols + j] += a->matrix_[i * order + k] * b->matrix_[k * cols + j];
        }
    return tmp;
}