#include <stdio.h>
#include "jacobi_method.h"
#define TOTAL_AREAS 320

double f(double x, double y, double z, double a) {
    return x * x + y * y + z * z;
}

double p(double x, double y, double z, double a) {
    return 6 - a * f(x, y, z, a);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double Dx = 2.0;
    double Dy = 2.0;
    double Dz = 2.0;
    double x = -1.0;
    double y = -1.0;
    double z = -1.0;
    double a = 100000, e = 1.0e-8;

    double start = MPI_Wtime();
    double **rez = MPI_Jacobi_method(Dx, Dy, Dz, TOTAL_AREAS / size, x, y, z, a, p, f, e, MPI_COMM_WORLD);
    double end = MPI_Wtime();

    double local_max_dev = 0;
    double global_max_dev = 0;
    double curr_dev;
    double hx = Dx / (TOTAL_AREAS - 1);
    double hy = Dy / (TOTAL_AREAS - 1);
    double hz = Dz / (TOTAL_AREAS - 1);
    int x_dimm_offset = rank * TOTAL_AREAS / size;
    for (int i = 0; i < TOTAL_AREAS / size; i++) {
        for (int j = 0; j < TOTAL_AREAS; j++) {
            for (int l = 0; l < TOTAL_AREAS; l++) {
                if((curr_dev = fabs(rez[i][j * TOTAL_AREAS + l] - f(x + (x_dimm_offset + i) * hx, y + j * hy, z + l * hz, a)))
                    > local_max_dev) {
                    local_max_dev = curr_dev;
                }
            }
        }
    }
    MPI_Allreduce(&local_max_dev, &global_max_dev, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    if(rank == 0) {
        printf("Maximal deviation: %.10lf\n", global_max_dev);
        printf("Total time: %lf sec.\n", end - start);
    }

    for(int i = 0; i < TOTAL_AREAS / size; i++) {
        free(rez[i]);
    }
    free(rez);
    MPI_Finalize();
    return 0;
}