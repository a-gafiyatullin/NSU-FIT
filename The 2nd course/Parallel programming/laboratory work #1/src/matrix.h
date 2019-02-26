#ifndef MATRIX
#define MATRIX

#include <stdio.h>  //fscanf, fprintf
#include <stdlib.h> //calloc, malloc
#include <string.h> //memcpy
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

//generate a random matrix
struct matrix* gen_matrix(int rows, int cols, float range);

//free the memory allocated for the matrix
void free_matrix(struct matrix* m);

//-----------------------------Matrix operations--------------------------------
//copy matrix from src to dest
void copy_matrix(struct matrix* dst, struct matrix* src);

//transpose the matrix
struct matrix* transpose_matrix(struct matrix* m);

//subtract the second operand from the first(from start to end)
//and assign the result to the first operand
void sub_matrices(struct matrix* a, struct matrix* b, int start, int end);

//matrix multiplication with saving result to c starting with "first_row" row
void mul_matrices(struct matrix* a, struct matrix* b, struct matrix* c, int first_row);

//matrix multiplication by a scalar
void mul_matrix_on_scalar(struct matrix* m, float scalar);

//get the squared euclid norm of the vector v
float squared_euclid_norm(struct matrix* v);

//get the matrix element
static inline float* get_element(struct matrix* m, int i, int j) {
    return &m->matrix_[i * (m->cols_ + m->cols_align_) + j];
};

//get the real size of the matrix
static inline int size_matrix(struct matrix* m) {
    return (m->rows_ + m->rows_align_) * (m->cols_ + m->cols_align_);
}

#endif //MATRIX