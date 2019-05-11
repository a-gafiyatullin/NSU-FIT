#include "jacobi_method.h"

void init_borders(double **current_val, double **prev_val, int areas_per_process, int rank, int size, val_func F,
        double a, double hx, double hy, double hz, double start_x, double start_y, double start_z);

double get_next_approx(double coeff, double x, double y, double z, double x_prev_val, double x_next_val,
        double y_prev_val, double y_next_val, double z_prev_val, double z_next_val,
        double hx, double hy, double hz, double a, val_func p);

double **MPI_Jacobi_method(double Dx, double Dy, double Dz, int areas_per_process, double start_x, double start_y,
                          double start_z, double a, val_func p, val_func F, double e, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank); /* get the environment info */

    int areas_per_dimm = size * areas_per_process;
    double hx = Dx / (areas_per_dimm - 1);   /* create the area split */
    double hy = Dy / (areas_per_dimm - 1);
    double hz = Dz / (areas_per_dimm - 1);
    double coeff = 1.0 / ((2.0 / (hx * hx)) + (2.0 / (hy * hy)) + (2.0 / (hz * hz)) + a);

    double **current_val = malloc(sizeof(double*) * areas_per_process); /* various approximation info */
    double **prev_val = malloc(sizeof(double*) * areas_per_process);
    int count = areas_per_dimm * areas_per_dimm;
    for(int i = 0; i < areas_per_process; i++) {
        current_val[i] = calloc(count, sizeof(double));
        prev_val[i] = calloc(count, sizeof(double));
    }
    double *prev_neighbour = malloc(count * sizeof(double));
    double *next_neighbour = malloc(count * sizeof(double));
    double global_max, curr, local_max;

    MPI_Request send_request[2], recv_request[2]; /* IPC info */

    init_borders(current_val, prev_val, areas_per_process, rank, size, F, a, hx, hy, hz, start_x, start_y, start_z);

    int x_dimm_offset = rank * areas_per_process;

    do {    /* main cycle */
        if(rank != size - 1) { /* init send and recv operations */
            MPI_Isend(current_val[areas_per_process - 1], count, MPI_DOUBLE, rank + 1, 0, comm, &send_request[NEXT_PROC]);
            MPI_Irecv(next_neighbour, count, MPI_DOUBLE, rank + 1, 1, comm, &recv_request[NEXT_PROC]);
        }
        if(rank != 0) {
            MPI_Isend(current_val[0], count, MPI_DOUBLE, rank - 1, 1, comm, &send_request[PREV_PROC]);
            MPI_Irecv(prev_neighbour, count, MPI_DOUBLE, rank - 1, 0, comm, &recv_request[PREV_PROC]);
        }
        /* calculate values in the inner cells while we are waiting for value of the borders */
        local_max = 0;
        for(int i = 1; i < areas_per_process - 1; ++i) {
            for(int j = 1; j < areas_per_dimm - 1; ++j) {
                for (int k = 1; k < areas_per_dimm - 1; ++k) {
                    prev_val[i][j * areas_per_dimm + k] = current_val[i][j * areas_per_dimm + k];
                    current_val[i][j * areas_per_dimm + k] = get_next_approx(coeff, start_x + (x_dimm_offset + i) * hx,
                            start_y + j * hy, start_z + k * hz,
                            prev_val[i - 1][j * areas_per_dimm + k], prev_val[i + 1][j * areas_per_dimm + k],
                            prev_val[i][(j - 1) * areas_per_dimm + k], prev_val[i][(j + 1) * areas_per_dimm + k],
                            prev_val[i][j * areas_per_dimm + k + 1], prev_val[i][j * areas_per_dimm + k - 1], hx, hy, hz,
                            a, p);
                    curr = fabs(current_val[i][j * areas_per_dimm + k] - prev_val[i][j * areas_per_dimm + k]);
                    if (curr > local_max) {
                        local_max = curr;
                    }
                }
            }
        }
        if(rank != size - 1) { /* check state of the operations */
            MPI_Wait(&recv_request[NEXT_PROC], MPI_STATUS_IGNORE);
            MPI_Wait(&send_request[NEXT_PROC], MPI_STATUS_IGNORE);
        }
        if(rank != size - 1) {
            for(int i = 1; i < areas_per_dimm - 1; ++i) {
                for (int j = 1; j < areas_per_dimm - 1; ++j) {
                    prev_val[areas_per_process - 1][i * areas_per_dimm + j]
                        = current_val[areas_per_process - 1][i * areas_per_dimm + j];
                    current_val[areas_per_process - 1][i * areas_per_dimm + j] = get_next_approx(coeff,
                            start_x + (x_dimm_offset + areas_per_process - 1) * hx, start_y + i * hy, start_z + j * hz,
                            prev_val[areas_per_process - 2][i * areas_per_dimm + j],
                            next_neighbour[i * areas_per_dimm + j],
                            prev_val[areas_per_process - 1][(i - 1) * areas_per_dimm + j],
                            prev_val[areas_per_process - 1][(i + 1) * areas_per_dimm + j],
                            prev_val[areas_per_process - 1][i * areas_per_dimm + j + 1],
                            prev_val[areas_per_process - 1][i * areas_per_dimm + j - 1], hx, hy, hz, a, p);
                    curr = fabs(current_val[areas_per_process - 1][i * areas_per_dimm + j]
                            - prev_val[areas_per_process - 1][i * areas_per_dimm + j]);
                    if (curr > local_max) {
                        local_max = curr;
                    }
                }
            }
        }
        if(rank != 0) {
            MPI_Wait(&recv_request[PREV_PROC], MPI_STATUS_IGNORE);
            MPI_Wait(&send_request[PREV_PROC], MPI_STATUS_IGNORE);
        }
        if(rank != 0) {   /* calculate a border cells */
            for(int i = 1; i < areas_per_dimm - 1; ++i) {
                for (int j = 1; j < areas_per_dimm - 1; ++j) {
                    prev_val[0][i * areas_per_dimm + j] = current_val[0][i * areas_per_dimm + j];
                    current_val[0][i * areas_per_dimm + j] = get_next_approx(coeff, start_x + x_dimm_offset * hx,
                            start_y + i * hy, start_z + j * hz, prev_neighbour[i * areas_per_dimm + j],
                            prev_val[1][i * areas_per_dimm + j], prev_val[0][(i - 1) * areas_per_dimm + j],
                            prev_val[0][(i + 1) * areas_per_dimm + j],
                            prev_val[0][i * areas_per_dimm + j + 1], prev_val[0][i * areas_per_dimm + j - 1],
                            hx, hy, hz, a, p);
                    curr = fabs(current_val[0][i * areas_per_dimm + j]
                                - prev_val[0][i * areas_per_dimm + j]);
                    if (curr > local_max) {
                        local_max = curr;
                    }
                }
            }
        }
        /* check the possibility of the ending */
        MPI_Allreduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if(global_max < e) {
            break;
        }
    } while(1);

    free(prev_neighbour);
    free(next_neighbour);
    for(int i = 0; i < areas_per_process; i++) {
        free(prev_val[i]);
    }
    free(prev_val);
    MPI_Barrier(MPI_COMM_WORLD);
    return current_val;
}

double get_next_approx(double coeff, double x, double y, double z, double x_prev_val, double x_next_val,
        double y_prev_val, double y_next_val, double z_prev_val, double z_next_val,
        double hx, double hy, double hz, double a, val_func p) {
    return coeff * (((x_next_val + x_prev_val) / (hx * hx)) + ((y_next_val + y_prev_val) / (hy * hy)) +
        ((z_next_val + z_prev_val) / (hz * hz)) - p(x, y, z, a));
}

void init_borders(double **current_val, double **prev_val, int areas_per_process, int rank, int size, val_func F,
        double a, double hx, double hy, double hz, double start_x, double start_y, double start_z) {

    int areas_per_dimm = size * areas_per_process;
    int x_dimm_offset = rank * areas_per_process;

    if(rank == 0) { /* calculate the borders values */
        for(int i = 0; i < areas_per_dimm; ++i) {
            for (int j = 0; j < areas_per_dimm; ++j) {
                current_val[0][i * areas_per_dimm + j] = F(start_x, start_y + i * hy, start_z + j * hz, a);
                prev_val[0][i * areas_per_dimm + j] = current_val[0][i * areas_per_dimm + j];
            }
        }
    }
    if(rank == size - 1) {  /* the same */
        for(int i = 0; i < areas_per_dimm; ++i) {
            for(int j = 0; j < areas_per_dimm; j++) {
                current_val[areas_per_process - 1][i * areas_per_dimm + j] = F(start_x + (areas_per_dimm - 1) * hx,
                        start_y + i * hy, start_z + j * hz, a);
                prev_val[areas_per_process - 1][i * areas_per_dimm + j]
                    = current_val[areas_per_process - 1][i * areas_per_dimm + j];
            }
        }
    }

    for(int i = 0; i < areas_per_process; ++i) {    /* calculate bottom, upper and border planes for each process */
        for (int j = 0; j < areas_per_dimm; ++j) {
            /* bottom plane */
            current_val[i][j * areas_per_dimm] = F(start_x + (x_dimm_offset + i) * hx, start_y + j * hy, start_z, a);
            prev_val[i][j * areas_per_dimm] = current_val[i][j * areas_per_dimm];

            /* upper plane */
            current_val[i][j * areas_per_dimm + areas_per_dimm - 1] = F(start_x + (x_dimm_offset + i) * hx, start_y + j * hy,
                    start_z + (areas_per_dimm - 1) * hz, a);
            prev_val[i][j * areas_per_dimm + areas_per_dimm - 1] = current_val[i][j * areas_per_dimm + areas_per_dimm - 1];

            /* nearest plane */
            current_val[i][j] = F(start_x + (x_dimm_offset + i) * hx, start_y, start_z + j * hz, a);
            prev_val[i][j] = current_val[i][j];

            /* far plane */
            current_val[i][areas_per_dimm * (areas_per_dimm - 1) + j] = F(start_x + (x_dimm_offset + i) * hx,
                    start_y + (areas_per_dimm - 1) * hy, start_z + j * hz, a);
            prev_val[i][areas_per_dimm * (areas_per_dimm - 1) + j]
                = current_val[i][areas_per_dimm * (areas_per_dimm - 1) + j];
        }
    }
}