#include "matrix.h"

struct matrix* create_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->align_ = (order % 4 == 0 ? 0 : 4 - (order % 4)); //calculate an alignment
    m->matrix_ = _mm_malloc(order * (order + m->align_) * sizeof(float), ALIGN); //allocate memory with alignment
    return m;
}

void get_matrix(struct matrix* m, FILE* in){
    register int order = m->order_;
    register int real_order = order + m->align_;
    for(int i = 0; i < order; i++)
        for(int j = 0; j < order; j++)
            fscanf(in, "%f", &m->matrix_[i * real_order + j]);
}

struct matrix* gen_matrix(int order, float range){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->align_ = (order % 4 == 0 ? 0 : 4 - (order % 4));
    register int real_order = order + m->align_;
    m->matrix_ = _mm_malloc(order * real_order * sizeof(float), ALIGN);
    srand(time(NULL));
    for(int i = 0; i < order; i++)
        for (int j = 0; j < order; j++)
            m->matrix_[i * real_order + j] = ((float)rand() / (float)(RAND_MAX)) * range;
    return m;
}

struct matrix* get_identity_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->align_ = (order % 4 == 0 ? 0 : 4 - (order % 4));
    register int real_order = order + m->align_;
    m->matrix_ = _mm_malloc(order *real_order * sizeof(float), ALIGN);
    memset(m->matrix_, 0, order * real_order * sizeof(float));
    for(int i = 0; i < order; i++)
        m->matrix_[i * real_order + i] = 1;
    return m;
}

void print_matrix(struct matrix* m, FILE* out){
    register int order = m->order_;
    register int real_order = order + m->align_;
    fprintf(out, "Order: %d\n", m->order_);
    for(int i = 0; i < order; i++){
        for(int j = 0; j < order; j++)
            fprintf(out, "%.4f ", m->matrix_[i * real_order + j]);
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void free_matrix(struct matrix* m){
    _mm_free(m->matrix_);
    free(m);
}

void copy_matrix(struct matrix* dst, struct matrix* src){
    memcpy(dst->matrix_, src->matrix_, src->order_ * (src->order_ + src->align_) * sizeof(float));
    dst->order_ = src->order_;
    dst->align_ = src->align_;
}

void transpose_matrix(struct matrix* m){
    register int order = m->order_;
    register int real_order = order + m->align_;
    for(int i = 0; i < order; i++)
        for(int j = i; j < order; j++){
            float tmp = m->matrix_[i * real_order + j];
            m->matrix_[i * real_order + j] = m->matrix_[j * real_order + i];
            m->matrix_[j * real_order + i] = tmp;
        }
}

void sum_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_ * (a->order_ + a->align_) / 4;
    __m128* xx = (__m128*)(a->matrix_);
    __m128* yy = (__m128*)(b->matrix_);
    for(int i = 0; i < order; i++)
            xx[i] = _mm_add_ps(xx[i], yy[i]);
}

void sub_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_ * (a->order_ + a->align_) / 4;
    __m128* xx = (__m128*)(a->matrix_);
    __m128* yy = (__m128*)(b->matrix_);
    for(int i = 0; i < order; i++)
        xx[i] = _mm_sub_ps(xx[i], yy[i]);
}

struct matrix* mul_matrices(struct matrix* a, struct matrix* b){
    register int order = a->order_;
    register int real_order = (order + a->align_) / 4;
    struct  matrix* tr_b = create_matrix(b->order_);
    copy_matrix(tr_b, b);
    transpose_matrix(tr_b); //transpose right matrix for better performance of multiplication
    struct matrix* tmp = create_matrix(order);
    __m128 p, sum;
    __m128* row = (__m128*)(a->matrix_);
    __m128* col = (__m128*)(tr_b->matrix_);
    for(int i = 0; i < order; i++)
        for(int j = 0; j < order; j++){
            sum = _mm_setzero_ps();
            for (int k = 0; k < real_order; k++){
                p = _mm_mul_ps(row[i * real_order + k], col[j * real_order + k]);
                sum = _mm_add_ps(sum, p);
            }
            p = _mm_movehl_ps(p, sum);
            sum = _mm_add_ps(sum, p);
            p = _mm_shuffle_ps(sum, sum, 1);
            sum = _mm_add_ss(sum, p);
            _mm_store_ss(&tmp->matrix_[i * real_order * 4 + j], sum);
        }
    free_matrix(tr_b);
    return tmp;
}

void mul_matrix_on_scalar(struct matrix* m, float scalar){
    float* div_row = _mm_malloc(4 * sizeof(float), ALIGN);
    for(int i = 0; i < 4; i++)
        div_row[i] = scalar;
    __m128* row = (__m128*)(m->matrix_);
    __m128* div = (__m128*)(div_row);
    register int order = m->order_ * (m->order_ + m->align_) / 4;
    for(int i = 0; i < order; i++)
            row[i] = _mm_mul_ps(row[i], *div);
    _mm_free(div_row);
}

void get_matrix_norms(struct matrix* m, float* l1_norm, float* max_norm){
    register int order = m->order_;
    register int real_order = order + m->align_;
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < order; i++){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < order; j++){
            row_sum += fabs(m->matrix_[i * real_order + j]);
            col_sum += fabs(m->matrix_[j * real_order + i]);
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