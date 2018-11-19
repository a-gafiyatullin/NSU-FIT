#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-------------------------------Matrix creation--------------------------------
struct matrix {
    float** matrix_;
    int order_;
};

//create an uninitialized matrix
struct matrix* create_matrix(int order);

//get a matrix from input stream
void get_matrix(struct matrix* m, FILE* in);

//get a zero matrix
struct matrix* get_zero_matrix(int order);

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


void div_matrix_on_scalar(struct matrix *m, float scalar){
    for(int i = 0; i < m->order_; i++)
        for(int j = 0; j < m->order_; j++)
            m->matrix_[i][j] /= scalar;
}

void get_matrix_norms(struct matrix *m, float* l1_norm, float* max_norm){
    *l1_norm = 0;
    *max_norm = 0;
    for(int i = 0; i < m->order_; i++){
        float row_sum = 0;
        float col_sum = 0;
        for(int j = 0; j < m->order_; j++){
            row_sum += m->matrix_[i][j];
            col_sum += m->matrix_[j][i];
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
    div_matrix_on_scalar(B, l1_norm * max_norm);
    printf("\n");
    struct matrix* R = get_identity_matrix(A->order_);
    struct matrix* BA = mul_matrices(B, A);
    sub_matrices(R, BA);
    free_matrix(BA);
    struct matrix* R_c = create_matrix(A->order_);
    copy_matrix(R_c, R);
    struct matrix* rev_A = get_identity_matrix(A->order_);
    for(int i = 0; i < iter_number; i++){
        sum_matrices(rev_A, R);
        R = mul_matrices(R_c, R);
    }
    rev_A = mul_matrices(rev_A, B);
    return rev_A;
}
