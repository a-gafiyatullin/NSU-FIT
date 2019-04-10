//
// Created by xp10rd on 4/10/19.
//

#ifndef MPI_MUL_MATRICES_H
#define MPI_MUL_MATRICES_H

#include <mpi.h>
#include <stdlib.h>
#include "matrix.h"

#define TRUE 1
#define FALSE 0
#define DIMS 2
#define X 0
#define Y 1

//distributed multiplication of the matrices
struct matrix *MPI_mul_matrices(struct matrix *A, struct matrix *B);

//create suitable matrix
void MPI_create_task(struct matrix **A, struct matrix **B, int cols_per_proc);

#endif //MPI_MUL_MATRICES_H
