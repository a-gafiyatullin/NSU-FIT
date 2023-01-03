#ifndef LINEAR_EQUATIONS
#define LINEAR_EQUATIONS

#include <mpi.h>
#include <math.h>   //sqrt
#include "matrix.h"
#define PI 3.14159265358979323846

//solve a system of linear equations Ax = b and return vector x(t -  parameter of the algorithm, eps - accuracy,
//recvcounts and displs - arrays for MPI_Allgatherv)
struct matrix* solve_system(struct matrix* A, struct matrix* b, int comm_size, int rank, float eps, float t,
        int* recvcounts, int * displs);

//get processing borders for using MPI functions
void get_borders(int dimm, int comm_size, int** recvcounts, int** displs);

//generate subsystem of liner equations for each process and return the solution
struct matrix* gen_system(struct matrix* A, struct matrix* b, int first);

#endif //LINEAR_EQUATIONS