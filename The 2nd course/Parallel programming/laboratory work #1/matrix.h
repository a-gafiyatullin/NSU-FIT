#ifndef MATRIX
#define MATRIX

#include <stdio.h>  //fscanf, fprintf
#include <stdlib.h> //calloc, malloc
#include <string.h> //memcpy
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

//matrix multiplication
struct matrix* mul_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication by a scalar
void mul_matrix_on_scalar(struct matrix* m, float scalar);

//matrix multiplication optimized for pre-transposed b
struct matrix* mul_matrices_tr_b(struct matrix* a, struct matrix* b);

#endif /* MATRIX */