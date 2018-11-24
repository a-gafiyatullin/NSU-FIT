#include "matrix.h"

struct matrix* create_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = calloc(order * order, sizeof(float));
    return m;
}

struct matrix* gen_matrix(int order, float range){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = calloc(order * order, sizeof(float));
    order *= order;
    srand(time(NULL));
    for(int i = 0; i < order; i++)
        m->matrix_[i] = ((float)rand()/(float)(RAND_MAX)) * range;
    return m;
}

void get_matrix(struct matrix* m, FILE* in){
    register int order = m->order_ * m->order_;
    for(int i = 0; i < order; i++)
        fscanf(in, "%f", &m->matrix_[i]);
}

struct matrix* get_identity_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->matrix_ = calloc(order * order, sizeof(float));
    for(int i = 0; i < order; i++)
        m->matrix_[i * order + i] = 1;
    return m;
}

void print_matrix(struct matrix* m, FILE* out){
    register int order = m->order_;
    fprintf(out, "Order: %d\n", m->order_);
    for(int i = 0; i < order; i++){
        for(int j = 0; j < order; j++)
            fprintf(out, "%.4f ", m->matrix_[i * order + j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void free_matrix(struct matrix* m){
    free(m->matrix_);
    free(m);
}

void copy_matrix(struct matrix* dst, struct matrix* src){
    memcpy(dst->matrix_, src->matrix_, sizeof(float)
        * src->order_ * src->order_);
    dst->order_ = src->order_;
}

void transpose_matrix(struct matrix* m){
    register int order = m->order_;
    for(int i = 0; i < order; i++)
        for(int j = i; j < order; j++){
            float tmp = m->matrix_[i * order + j];
            m->matrix_[i * order + j] = m->matrix_[j * order + i];
            m->matrix_[j * order + i] = tmp;
    }
}

void sum_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_ * a->order_;
    for(int i = 0; i < order; i++)
        a->matrix_[i] += b->matrix_[i];
}

void sub_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_ * a->order_;
    for(int i = 0; i < order; i++)
        a->matrix_[i] -= b->matrix_[i];
}

struct matrix* mul_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_;
    struct matrix* tmp = create_matrix(order);
    for(int i = 0; i < order; i++)
        for(int j = 0; j < order; j++)
            for(int k = 0; k < order; k++)
                tmp->matrix_[i * order + j] += a->matrix_[i * order + k]
                    * b->matrix_[k * order + j];
    return tmp;
}

void mul_matrix_on_scalar(struct matrix* m, float scalar){
    register int order = m->order_ * m->order_;
    for(int i = 0; i < order; i++)
        m->matrix_[i] *= scalar;
}

void get_matrix_norms(struct matrix* m, float* l1_norm, float* max_norm){
    register int order = m->order_;
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < order; i++){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < order; j++){
            row_sum += m->matrix_[i * order + j];
            col_sum += m->matrix_[j * order + i];
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
