#include <stdio.h>
#include <stdlib.h>

struct matrix {
    float** matrix_;
    int order_;
    int l1Norm_;
    int maxNorm_;
};

struct matrix* create_matrix(int order){
    struct matrix* m = malloc(sizeof(struct matrix));
    m->order_ = order;
    m->l1Norm_ = 0;
    m->matrix_ = malloc(sizeof(float*) * order);
    for(int i = 0; i < order; i++)
        m->matrix_[i] = malloc(sizeof(float) * order);
    return m;
}

void get_matrix(struct matrix* m, FILE *in){
    for(int i = 0; i < m->order_; i++){
        int currentNorm = 0;
        for(int j = 0; j < m->order_; j++){
            fscanf(in, "%f", &m->matrix_[i][j]);
            currentNorm += m->matrix_[i][j];
        }
        if(currentNorm > m->l1Norm_)
            m->l1Norm_ = currentNorm;
    }
}

void print_matrix(struct matrix* m, FILE *out){
    for(int i = 0; i < m->order_; i++){
        for(int j = 0; j < m->order_; j++)
            fprintf(out, "%.2f ", m->matrix_[i][j]);
        fprintf(out, "\n");
    }
}

struct matrix* transpose_matrix(struct matrix* m){
    struct matrix* tmp = create_matrix(m->order_);
    for(int i = 0; i < m->order_; i++){
        int currentNorm = 0;
        for(int j = 0; j < m->order_; j++){
            tmp->matrix_[i][j] = m->matrix_[j][i];
            currentNorm += m->matrix_[j][i];
        }
        if(currentNorm > m->maxNorm_)
            m->maxNorm_ = currentNorm;
    }
    tmp->l1Norm_ = m->maxNorm_;
    tmp->maxNorm_ = m->l1Norm_;
    return tmp;
}

void sum_matrix(struct matrix* a, struct matrix* b){
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            a->matrix_[i][j] += b->matrix_[i][j];
}

void sub_matrix(struct matrix* a, struct matrix* b){
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            a->matrix_[i][j] -= b->matrix_[i][j];
}

struct matrix* mul_matrix(struct matrix* a, struct matrix* b, struct matrix* rez){
    for(int i = 0; i < a->order_; i++)
        for(int j = 0; j < a->order_; j++)
            for(int k = 0; k < a->order_; k++)
                rez->matrix_[i][j] += a->matrix_[i][k] * b->matrix_[k][j];
}

int main(){
    struct matrix* m1 = create_matrix(3);
    get_matrix(m1, stdin);
    struct matrix* m2 = create_matrix(3);
    get_matrix(m2, stdin);
    sum_matrix(m1, m2);
    print_matrix(m1, stdout);
    return 0;
}
