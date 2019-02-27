#ifndef LINEAR_EQUATIONS
#define LINEAR_EQUATIONS

#include <mpi.h>
#include <math.h>   //for sqrt
#include "matrix.h"
#define PI 3.14159265358979323846

//solve a system of linear equations Ax = b and return vector x - part of solution for this process
//(t -  parameter of the algorithm, eps - accuracy,
//start_pos - number of starting line for this process in the source matrix)
struct matrix* solve_system(struct matrix* A, struct matrix* b, float eps, float t, int first, int last);

//get processing borders for using MPI_Allgatherv
void get_borders(struct matrix* m, int comm_size, int** recvcounts, int** displs);

//generate subsystem of liner equations for each process
void gen_system(struct matrix* A, struct matrix* b, int first);

#endif //LINEAR_EQUATIONS