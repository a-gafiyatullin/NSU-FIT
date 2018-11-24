#include <stdio.h>  //for fscanf/fprintf
#include <stdlib.h> //for calloc/malloc
#include <string.h> //for memcpy
#include <time.h>   //for time
#include <unistd.h> //for sysconf
#include <sys/times.h>//for times

//-------------------------------Matrix creation--------------------------------
struct matrix {
    float* matrix_;
    int order_;
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
void free_matrix(struct matrix* m);

//-----------------------------Matrix operations--------------------------------
//copy matrix from src to dest
void copy_matrix(struct matrix* dst, struct matrix* src);

//transpose the matrix
void transpose_matrix(struct matrix* m);

//summarize two matrices and assign the result to the first operand
void sum_matrices(struct matrix* a, struct matrix* b);

//subtract the second operand from the first
//and assign the result to the first operand
void sub_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication
struct matrix* mul_matrices(struct matrix* a, struct matrix* b);

//matrix multiplication by a scalar
void mul_matrix_on_scalar(struct matrix* m, float scalar);

//get l1 and maximal matrix norms
void get_matrix_norms(struct matrix* m, float* l1_norm, float* max_norm);

//invert the matrix
struct matrix* invert_matrix(struct matrix* A, int iter_number);
