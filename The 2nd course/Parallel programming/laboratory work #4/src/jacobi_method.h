#ifndef JACOBI_METHOD_H
#define JACOBI_METHOD_H

#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define PREV_PROC 0
#define NEXT_PROC 1

typedef double (*val_func)(double x, double y, double z, double a);

/* get function values in the area */
double **MPI_Jacobi_method(double Dx, double Dy, double Dz, int areas_per_process, double start_x, double start_y,
        double start_z, double a, val_func p, val_func F, double e, MPI_Comm comm);

#endif //JACOBI_METHOD_H