#include <stdio.h>  //fscanf, fprintf
#include <stdlib.h> //calloc, malloc
#include <string.h> //memcpy
#include <time.h>   //time
#include <unistd.h> //sysconf
#include <sys/times.h>//times
#include <xmmintrin.h>  //operations with vectors
#include <math.h>   // fabsf
#define  ALIGN 16
//-------------------------------Matrix creation--------------------------------
struct matrix{
    int order_;
    int align_; //align matrix_'s ending to 16 bytes
    float* matrix_;
};

//create an uninitialized matrix
struct matrix* create_matrix(int order);

//get a matrix from input stream
void get_matrix(struct matrix* m, FILE* in);

//generate a random matrix
struct matrix* gen_matrix(int order, float range);

//get a get identity matrix
struct matrix* get_identity_matrix(int order);

//print the matrix in the output stream
void print_matrix(struct matrix* m, FILE* out);

//free the memory allocated for the matrix
inline void free_matrix(struct matrix* m);

//-----------------------------Matrix operations--------------------------------
//copy matrix from src to dest
void copy_matrix(struct matrix* dst, struct matrix* src);

//transpose the matrix
void transpose_matrix(struct matrix* m);

//summarize two matrices and assign the result to the first operand
inline void sum_matrices(struct matrix* a, struct matrix* b);

//subtract the second operand from the first
//and assign the result to the first operand
inline void sub_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication
struct matrix* mul_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication by a scalar
void mul_matrix_on_scalar(struct matrix* m, float scalar);

//matrix multiplication optimized for pre-transposed b
struct matrix* mul_matrices_tr_b(struct matrix* a, struct matrix* b);

//get l1 and maximal matrix norms
void get_matrix_norms(struct matrix* m, float* l1_norm, float* max_norm);

//invert the matrix
struct matrix* invert_matrix(struct matrix* A, int iter_number);