#include "matrix.h"

struct matrix* create_matrix(int rows, int cols) {
    struct matrix* m = malloc(sizeof(struct matrix));
    m->rows_ = rows;
    m->cols_ = cols;
    m->rows_align_ = (rows % 4 == 0 ? 0 : 4 - (rows % 4)); //calculate an alignment
    m->cols_align_ = (cols % 4 == 0 ? 0 : 4 - (cols % 4));
    //allocate memory with alignment
    m->matrix_ = _mm_malloc((m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_) * sizeof(float), ALIGN);
    return m;
}

void get_matrix(struct matrix* m, FILE* in) {
    register int rows = m->rows_;
    register int cols = m->cols_;
    register int col_real_order = cols + m->cols_align_;
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; ++j) {
            fscanf(in, "%f", &m->matrix_[i * col_real_order + j]);
        }
}

void print_matrix(struct matrix* m, FILE* out) {
    register int rows = m->rows_;
    register int cols = m->cols_;
    register int col_real_order = cols + m->cols_align_;
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; j++) {
            fprintf(out, "%.4f ", m->matrix_[i * col_real_order + j]);
        }
        fprintf(out, "\n");
    }
    fprintf(out, "\n");
}

void free_matrix(struct matrix* m) {
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
    register int col_real_order = cols + m->cols_align_;
    register int row_real_order = rows + m->rows_align_;
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            tr_m->matrix_[j * row_real_order + i] = m->matrix_[i * col_real_order + j];
        }
    }
    return tr_m;
}

void sub_matrices(struct matrix* a, struct matrix* b) {
    register int order = (a->rows_ + a->rows_align_) * (a->cols_ + a->cols_align_) / 4;
    __m128* xx = (__m128*)(a->matrix_);
    __m128* yy = (__m128*)(b->matrix_);
    for(int i = 0; i < order; ++i) {
        xx[i] = _mm_sub_ps(xx[i], yy[i]);
    }
}

struct matrix* mul_matrices(struct matrix* a, struct matrix* b) {
    register int rows = a->rows_;
    register int cols = b->cols_;
    register int real_order = (a->cols_ + a->cols_align_) / 4;
    struct  matrix* tr_b = transpose_matrix(b); //transpose right matrix for better performance of multiplication
    struct matrix* tmp = create_matrix(rows, cols);
    __m128 p, sum;
    __m128* row = (__m128*)(a->matrix_);
    __m128* col = (__m128*)(tr_b->matrix_);
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sum = _mm_setzero_ps();
            for (int k = 0; k < real_order; ++k) {
                p = _mm_mul_ps(row[i * real_order + k], col[j * real_order + k]);
                sum = _mm_add_ps(sum, p);
            }
            p = _mm_movehl_ps(p, sum);
            sum = _mm_add_ps(sum, p);
            p = _mm_shuffle_ps(sum, sum, 1);
            sum = _mm_add_ss(sum, p);
            _mm_store_ss(&tmp->matrix_[i * real_order * 4 + j], sum);
        }
    }
    free_matrix(tr_b);
    return tmp;
}

struct matrix* mul_matrices_tr_b(struct matrix* a, struct matrix* b) {
    register int rows = a->rows_;
    register int cols = b->rows_;
    register int real_order = (a->cols_ + a->cols_align_) / 4;
    struct matrix* tmp = create_matrix(rows, cols);
    __m128 p, sum;
    __m128* row = (__m128*)(a->matrix_);
    __m128* col = (__m128*)(b->matrix_);
    for(int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sum = _mm_setzero_ps();
            for (int k = 0; k < real_order; ++k) {
                p = _mm_mul_ps(row[i * real_order + k], col[j * real_order + k]);
                sum = _mm_add_ps(sum, p);
            }
            p = _mm_movehl_ps(p, sum);
            sum = _mm_add_ps(sum, p);
            p = _mm_shuffle_ps(sum, sum, 1);
            sum = _mm_add_ss(sum, p);
            _mm_store_ss(&tmp->matrix_[i * real_order * 4 + j], sum);
        }
    }
    return tmp;
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