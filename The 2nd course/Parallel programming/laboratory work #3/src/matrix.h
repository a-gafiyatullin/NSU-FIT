//
// Created by xp10rd on 4/1/19.
//

#ifndef MATRIX_H

#include <stdio.h>
#include <stdlib.h> //malloc
#include <assert.h> //assert
#include <time.h>

struct matrix {
    double *matrix_;
    int rows_;
    int cols_;
};

//-------------------------------Matrix creation--------------------------------

//create an uninitialized matrix
struct matrix *create_matrix(int rows, int cols);

//get a matrix from input stream
void get_matrix(struct matrix* m, FILE* in);

//print the matrix in the output stream
void print_matrix(struct matrix *m, FILE *out);

//free the memory allocated for the matrix
void free_matrix(struct matrix *m);

//generate random matrix
struct matrix *gen_matrix(int rows, int cols);

//-----------------------------Matrix operations--------------------------------

//transpose the matrix
struct matrix *transpose_matrix(struct matrix* m);

//matrix multiplication
struct matrix *mul_matrices(struct matrix* a, struct matrix* b);

#define MATRIX_H

#endif //MATRIX_H
