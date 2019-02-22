#ifndef LINEAR_EQUATIONS
#define LINEAR_EQUATIONS

#include <mpi.h>
#include "matrix.h"
#define PI 3.14159265358979323846

//solve a system of linear equations Ax = b and return vector x(t -  parameter of the algorithm, eps - accuracy)
struct matrix* solve_system(struct matrix* A, struct matrix* b, int comm_size, int rank, float eps, float t);

//get processing borders for using MPI_Allgatherv
void get_borders(struct matrix* m, int comm_size, int** recvcounts, int** displs);

//generate simple system of liner equations and return its answer
struct matrix* gen_system(struct matrix* A, struct matrix* b);

#endif /* LINEAR_EQUATIONS */