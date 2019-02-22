#ifndef MATRIX
#define MATRIX

#include <stdio.h>  //fscanf, fprintf
#include <stdlib.h> //calloc, malloc
#include <string.h> //memcpy
#include <math.h>   //sqrt
#include <time.h> //for random
#include <xmmintrin.h>  //operations with vectors
#define  ALIGN 16
//-------------------------------Matrix creation--------------------------------
struct matrix {
    int rows_;
    int cols_;
    int rows_align_; //align matrix_'s ending to 16 bytes
    int cols_align_;
    float* matrix_;
};

//create an uninitialized matrix
struct matrix* create_matrix(int rows, int cols);

//get a matrix from input stream
void get_matrix(struct matrix* m, FILE* in);

//print the matrix in the output stream
void print_matrix(struct matrix* m, FILE* out);

//generate matrix
struct matrix* gen_matrix(int rows, int cols);

//free the memory allocated for the matrix
void free_matrix(struct matrix* m);

//-----------------------------Matrix operations--------------------------------
//copy matrix from src to dest
void copy_matrix(struct matrix* dst, struct matrix* src);

//transpose the matrix
struct matrix* transpose_matrix(struct matrix* m);

//subtract the second operand from the first
//and assign the result to the first operand
void sub_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication from the "first_row" row to the "last_row" row(not included)
struct matrix* mul_matrices(struct matrix* a, struct matrix* b, int first_row, int last_row);

//matrix multiplication by a scalar
void mul_matrix_on_scalar(struct matrix* m, float scalar);

//matrix multiplication optimized for pre-transposed b from the "first_row" row to the "last_row" row(not included)
struct matrix* mul_matrices_tr_b(struct matrix* a, struct matrix* b, int first_row, int last_row);

//get euclid norm of the vector v */
float euclid_norm(struct matrix* v);

//get matrix element
static inline float* get_element(struct matrix* m, int i, int j) {
    return &m->matrix_[i * (m->cols_ + m->cols_align_) + j];
};

//get real size of the matrix */
static inline int size_matrix(struct matrix* m) {
    return (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_);
}

#endif /* MATRIX */