#include "matrix.h"

struct matrix* create_matrix(int rows, int cols) {
    struct matrix* m = non_init_create_matrix(rows, cols);
    register int order = (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_);
    for(int i = 0; i < order; i++) {
        m->matrix_[i] = 0;
    }
    return m;
}

struct matrix* non_init_create_matrix(int rows, int cols) {
    struct matrix* m = malloc(sizeof(struct matrix));
    m->rows_ = rows;
    m->cols_ = cols;
    m->rows_align_ = (rows % 4 == 0 ? 0 : 4 - (rows % 4)); //calculate an alignment
    m->cols_align_ = (cols % 4 == 0 ? 0 : 4 - (cols % 4));
    //allocate memory with alignment
    m->matrix_ = _mm_malloc((m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_) * sizeof(float), ALIGN);
    register int order = (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_);
    return m;
}

void get_matrix(struct matrix* m, FILE* in) {
    register int rows = m->rows_;
    register int cols = m->cols_;
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; ++j) {
            fscanf(in, "%f", get_element(m, i, j));
        }
}

void print_matrix(struct matrix* m, FILE* out) {
    register int rows = m->rows_;
    register int cols = m->cols_;
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; j++) {
            fprintf(out, "%.4f ", *get_element(m, i, j));
        }
        fprintf(out, "\n");
    }
}

struct matrix* gen_matrix(int rows, int cols, float range) {
    srand(time(NULL));
    struct matrix* tmp = create_matrix(rows, cols);
    for(int i = 0; i < tmp->rows_; i++) {
        for(int j = 0; j < tmp->cols_; j++) {
            *get_element(tmp, i, j) = ((float)rand() / (float)(RAND_MAX)) * range;
        }
    }
    return tmp;
}

void free_matrix(struct matrix* m) {
    if(m == NULL)
        return;
    _mm_free(m->matrix_);
    free(m);
}

void copy_matrix(struct matrix* dst, struct matrix* src) {
    memcpy(dst->matrix_, src->matrix_, (src->rows_ + src->rows_align_) * (src->cols_ + src->cols_align_) * sizeof(float));
    dst->rows_ = src->rows_;
    dst->cols_ = src->cols_;
    dst->rows_align_ = src->rows_align_;
    dst->cols_align_ = src->cols_align_;
}

struct matrix* transpose_matrix(struct matrix* m) {
    struct matrix* tr_m = create_matrix(m->cols_, m->rows_);
    register int rows = m->rows_;
    register int cols = m->cols_;
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            *get_element(tr_m, j, i) = *get_element(m, i, j);
        }
    }
    return tr_m;
}

void sub_matrices(struct matrix* a, struct matrix* b) {
    register int order = (a->rows_+ a->rows_align_) * (a->cols_ + a->cols_align_) / 4;
    __m128* xx = (__m128*)(a->matrix_);
    __m128* yy = (__m128*)(b->matrix_);
    for(int i = 0; i < order; ++i)
        xx[i] = _mm_sub_ps(xx[i], yy[i]);
}

void mul_matrices(struct matrix* a, struct matrix* b, struct matrix* c, int first_col) {
    register int rows = a->rows_;
    register int cols = b->cols_;
    struct  matrix* tr_b = transpose_matrix(b); //transpose right matrix for better performance of multiplication
    register int ans_real_order = (c->cols_ + c->cols_align_);
    register int a_real_order = (a->cols_ + a->cols_align_);
    register int b_real_order = (tr_b->cols_ + tr_b->cols_align_);
    for(int i = 0; i < rows; ++i) {
        register int ans_order = i * ans_real_order;
        register int a_order = i * a_real_order;
        for (int j = 0; j < cols; ++j) {
            register int b_order = j * b_real_order;
            for(int k = first_col; k < a->cols_; k++) {
                c->matrix_[ans_order + j] += a->matrix_[a_order + k] * tr_b->matrix_[b_order + k - first_col];
            }
        }
    }
    free_matrix(tr_b);
}

void mul_matrices_tr_b(struct matrix* a, struct matrix* b, struct matrix* c, int first_col) {
    register int rows = a->rows_;
    register int cols = b->rows_;
    register int ans_real_order = (c->cols_ + c->cols_align_);
    register int a_real_order = (a->cols_ + a->cols_align_);
    register int b_real_order = (b->cols_ + b->cols_align_);
    for(int i = 0; i < rows; ++i) {
        register int ans_order = i * ans_real_order;
        register int a_order = i * a_real_order;
        for (int j = 0; j < cols; ++j) {
            register int b_order = j * b_real_order;
            for(int k = first_col; k < a->cols_; k++) {
                c->matrix_[ans_order + j] += a->matrix_[a_order + k] * b->matrix_[b_order + k - first_col];
            }
        }
    }
}

void mul_matrix_on_scalar(struct matrix* m, float scalar) {
    float* div_row = _mm_malloc(4 * sizeof(float), ALIGN);
    for(int i = 0; i < 4; ++i)
        div_row[i] = scalar;
    __m128* row = (__m128*)(m->matrix_);
    __m128* div = (__m128*)(div_row);
    register int order = (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_) / 4;
    for(int i = 0; i < order; ++i) {
        row[i] = _mm_mul_ps(row[i], *div);
    }
    _mm_free(div_row);
}

float squared_euclid_norm(struct matrix* m) {
    __m128 sum = _mm_setzero_ps();
    __m128 p;
    __m128* vector = (__m128*)(m->matrix_);
    float rez = 0;
    register int order = (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_) / 4;
    for (int i = 0; i < order; ++i) {
        p = _mm_mul_ps(vector[i], vector[i]);
        sum = _mm_add_ps(sum, p);
    }
    p = _mm_movehl_ps(p, sum);
    sum = _mm_add_ps(sum, p);
    p = _mm_shuffle_ps(sum, sum, 1);
    sum = _mm_add_ss(sum, p);
    _mm_store_ss(&rez, sum);
    return rez;
}