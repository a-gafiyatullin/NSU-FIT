//
// Created by xp10rd on 4/29/19.
//

#ifndef JACOBI_METHOD_H
#define JACOBI_METHOD_H

#include <mpi.h>
#include <stdlib.h>
#include <math.h>

#define PREV_PROC 0
#define NEXT_PROC 1

typedef struct area {
    double Dx;
    double Dy;
    double Dz;
} area;

typedef struct point {
    double x;
    double y;
    double z;
} point;

typedef struct values {
    point *p;
    double *value;
} values;

typedef double (*val_func)(point point, double a);

/* get function values in the area */
values *MPI_Jacobi_method(area cube, int areas_per_process, point start_point, double a, val_func p, val_func F,
        double e, MPI_Comm comm);

#endif //JACOBI_METHOD_H