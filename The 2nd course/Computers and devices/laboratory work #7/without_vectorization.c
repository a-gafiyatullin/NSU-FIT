#include "matrix.h"

struct matrix* create_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = malloc(sizeof(float*) * order);
    for(int i = 0; i < order; i++)
        m->matrix_[i] = malloc(sizeof(float) * order);
    return m;
}

void get_matrix(struct matrix* m, FILE* in){
    for(int i = 0; i < m->order_; i++)
        for(int j = 0; j < m->order_; j++)
            fscanf(in, "%f", &m->matrix_[i][j]);
}

struct matrix* get_zero_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = malloc(sizeof(float*) * order);
    for(int i = 0; i < order; i++)
        m->matrix_[i] = calloc(order, sizeof(float));
    return m;
}

struct matrix* get_identity_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = malloc(sizeof(float*) * order);
    for(int i = 0; i < order; i++){
        m->matrix_[i] = calloc(order, sizeof(float));
        m->matrix_[i][i] = 1;
    }
    return m;
}

void print_matrix(struct matrix* m, FILE* out){
    for(int i = 0; i < m->order_; i++){
        for(int j = 0; j < m->order_; j++)
            fprintf(out, "%.4f ", m->matrix_[i][j]);
        fprintf(out, "\n");
    }
}

void free_matrix(struct matrix* m){
    for(int i = 0; i < m->order_; i++)
        free(m->matrix_[i]);
    free(m->matrix_);
    free(m);
}

void copy_matrix(struct matrix* dst, struct matrix* src){
    for(int i = 0; i < src->order_; i++)
        memcpy(dst->matrix_[i], src->matrix_[i], sizeof(float) * src->order_);
}

void transpose_matrix(struct matrix* m){
    for(int i = 0; i < m->order_; i++)
        for(int j = i; j < m->order_; j++){
            float tmp = m->matrix_[i][j];
            m->matrix_[i][j] = m->matrix_[j][i];
            m->matrix_[j][i] = tmp;
    }
}

void sum_matrices(struct matrix* a, struct matrix* b){
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            a->matrix_[i][j] += b->matrix_[i][j];
}

void sub_matrices(struct matrix* a, struct matrix* b){
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            a->matrix_[i][j] -= b->matrix_[i][j];
}

struct matrix* mul_matrices(struct matrix* a, struct matrix* b){
    struct matrix* tmp = get_zero_matrix(a->order_);
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            for(int k = 0; k < a->order_; k++)
                tmp->matrix_[i][j] += a->matrix_[i][k] * b->matrix_[k][j];
    return tmp;
}
