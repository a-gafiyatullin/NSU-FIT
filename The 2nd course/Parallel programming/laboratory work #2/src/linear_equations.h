#ifndef LINEAR_EQUATIONS
#define LINEAR_EQUATIONS

#include <omp.h>
#include <math.h>   //sqrtf
#include "matrix.h"
#define PI 3.14159265358979323846f

//solve a system of linear equations Ax = b and return vector x(t -  parameter of the algorithm, eps - accuracy)
struct matrix* solve_system(struct matrix* A, struct matrix* b, float eps, float t);

//generate system of linear equations
struct matrix* gen_system(struct matrix* A, struct matrix* b);

#endif //LINEAR_EQUATIONS