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
    fprintf(out, "Order: %d\n", m->order_);
    for(int i = 0; i < m->order_; i++){
        for(int j = 0; j < m->order_; j++)
            fprintf(out, "%.4f ", m->matrix_[i][j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
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
    dst->order_ = src->order_;
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

void mul_matrix_on_scalar(struct matrix* m, float scalar){
    for(int i = 0; i < m->order_; i++)
        for(int j = 0; j < m->order_; j++)
            m->matrix_[i][j] *= scalar;
}

void get_matrix_norms(struct matrix* m, float* l1_norm, float* max_norm){
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < m->order_; i++){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < m->order_; j++){
            row_sum += m->matrix_[i][j];
            col_sum += m->matrix_[j][i];
        }
        if(*l1_norm < col_sum)
            *l1_norm = col_sum;
        if(*max_norm < row_sum)
            *max_norm = row_sum;
    }
}

struct matrix* invert_matrix(struct matrix* A, int iter_number){
    struct matrix* B = create_matrix(A->order_);
    copy_matrix(B, A);
    transpose_matrix(B);
    float l1_norm, max_norm;
    get_matrix_norms(A, &l1_norm, &max_norm);
    mul_matrix_on_scalar(B, 1.0 / (l1_norm * max_norm));
    struct matrix* R = get_identity_matrix(A->order_);
    struct matrix* BA = mul_matrices(B, A);
    sub_matrices(R, BA);
    free_matrix(BA);
    struct matrix* R_1_deg = create_matrix(A->order_);
    copy_matrix(R_1_deg, R);
    struct matrix* inv_A = get_identity_matrix(A->order_);
    struct matrix* tmp;
    for(int i = 0; i < iter_number; i++){
        sum_matrices(inv_A, R);
        tmp = mul_matrices(R, R_1_deg);
        free_matrix(R);
        R = tmp;
    }
    tmp = mul_matrices(inv_A, B);
    free_matrix(inv_A);
    inv_A = tmp;
    free_matrix(B);
    free_matrix(R);
    free_matrix(R_1_deg);
    return inv_A;
}
