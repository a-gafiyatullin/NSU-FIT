#include <stdio.h>
#include "jacobi_method.h"
#define TOTAL_AREAS 50000

double f(point p, double a) {
    return p.x * p.x + p.y * p.y + p.z * p.z;
}

double p(point p, double a) {
    return 6 - a * f(p, a);
}

void print_values(values *val, int size) {
    for(int i = 0; i < size; i++) {
        printf("point (%lf, %lf, %lf) = %lf\n", val->p[i].x, val->p[i].y, val->p[i].z, val->value[i]);
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    area cube;
    cube.Dx = 2;
    cube.Dy = 2;
    cube.Dz = 2;
    point start_point;
    start_point.x = -1;
    start_point.y = -1;
    start_point.z = -1;
    double a = 100000, e = 1.0e-8;

    double start = MPI_Wtime();
    values *rez = MPI_Jacobi_method(cube, TOTAL_AREAS / size, start_point, a, p, f, e, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    int turn = 0;
    if(size != 1) {
        if (rank == 0) {
            print_values(rez, TOTAL_AREAS / size);
            MPI_Send(&turn, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        } else if (rank != size - 1) {
            MPI_Recv(&turn, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_values(rez, TOTAL_AREAS / size);
            MPI_Send(&turn, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD);
        } else {
            MPI_Recv(&turn, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            print_values(rez, TOTAL_AREAS / size);
            printf("Total time: %lf\n", end - start);
        }
    } else {
        print_values(rez, TOTAL_AREAS / size);
        printf("Total time: %lf\n", end - start);
    }
    free(rez->value);
    free(rez->p);
    free(rez);
    MPI_Finalize();
    return 0;
}