//
// Created by xp10rd on 4/29/19.
//
#include "jacobi_method.h"

/* get the pivot points for the process with rank "rank" */
void init_local_points(int areas_per_process, point *local_points, point start_point, double hx, double hy, double hz,
        int rank);

double get_next_approx(double coeff, const double *values, double hx, double hy, double hz, point point, double a,
        val_func p);

values *MPI_Jacobi_method(area cube, int areas_per_process, point start_point, double a, val_func p, val_func F,
        double e, MPI_Comm comm) {
    int size, rank;
    MPI_Comm_size(comm, &size);
    MPI_Comm_rank(comm, &rank); /* get the environment info */

    double hx = cube.Dx / (size * areas_per_process - 1);   /* create the area split */
    double hy = cube.Dy / 2;
    double hz = cube.Dz / 2;
    point *local_points = malloc(sizeof(point) * areas_per_process);
    init_local_points(areas_per_process, local_points, start_point, hx, hy, hz, rank);
    double coeff = 1.0 / ((2.0 / (hx * hx)) + (2.0 / (hy * hy)) + (2.0 / (hz * hz)) + a);

    double *current_val = calloc(areas_per_process, sizeof(double));    /* various approximation info */
    double *prev_val = calloc(areas_per_process, sizeof(double));
    double prev_neighbour = 0, next_neighbour = 0, global_max, curr, local_max;

    MPI_Request send_request[2], recv_request[2]; /* IPC info */

    /* rezult info */
    values *ended_values = malloc(sizeof(values));
    char *ended_flags = malloc(sizeof(char) * size);

    if(rank == 0) { /* calculate the borders values */
        current_val[0] = F(local_points[0], a);
        prev_val[0] = current_val[0];   /* because it's true value of the function */
    }
    if(rank == size - 1) {  /* the same */
        current_val[areas_per_process - 1] = F(local_points[areas_per_process - 1], a);
        prev_val[areas_per_process - 1] = current_val[areas_per_process - 1];
    }
    do {    /* main cycle */
        if(rank != size - 1) { /* init send and recv operations */
            MPI_Isend(&current_val[areas_per_process - 1], 1, MPI_DOUBLE, rank + 1, 0, comm, &send_request[NEXT_PROC]);
            MPI_Irecv(&next_neighbour, 1, MPI_DOUBLE, rank + 1, 1, comm, &recv_request[NEXT_PROC]);
        }
        if(rank != 0) {
            MPI_Isend(current_val, 1, MPI_DOUBLE, rank - 1, 1, comm, &send_request[PREV_PROC]);
            MPI_Irecv(&prev_neighbour, 1, MPI_DOUBLE, rank - 1, 0, comm, &recv_request[PREV_PROC]);
        }
        /* calculate values in the inner cells while we are waiting for value of the borders */
        local_max = 0;
        for (int i = 1; i < areas_per_process - 1; i++) {
            prev_val[i] = current_val[i];
            current_val[i] = get_next_approx(coeff, prev_val + i, hx, hy, hz, local_points[i], a, p);
            curr = fabs(current_val[i] - prev_val[i]);
            if (curr > local_max) {
                local_max = curr;
            }
        }
        if(rank != size - 1) { /* check state of the operations */
            MPI_Wait(&recv_request[NEXT_PROC], MPI_STATUS_IGNORE);
            MPI_Wait(&send_request[NEXT_PROC], MPI_STATUS_IGNORE);
        }
        if(rank != 0) {
            MPI_Wait(&recv_request[PREV_PROC], MPI_STATUS_IGNORE);
            MPI_Wait(&send_request[PREV_PROC], MPI_STATUS_IGNORE);
        }
        if(rank != 0) {   /* calculate a border cells */
            prev_val[0] = current_val[0];
            current_val[0] = coeff * ((prev_val[1] + prev_neighbour) / (hx * hx) + 2 * prev_val[0] / (hy * hy) +
                    2 * prev_val[0] / (hz * hz) - p(local_points[0], a));
        }
        if(rank != size - 1) {
            prev_val[areas_per_process - 1] = current_val[areas_per_process - 1];
            current_val[areas_per_process - 1] = coeff * ((prev_val[areas_per_process - 2] + next_neighbour)
                    / (hx * hx) + 2 * prev_val[areas_per_process - 1] / (hy * hy) +
                    2 * prev_val[areas_per_process - 1] / (hz * hz) - p(local_points[areas_per_process - 1], a));
        }
        if(local_max < fabs(current_val[0] - prev_val[0])) {
            local_max = fabs(current_val[0] - prev_val[0]);
        }
        if(local_max < fabs(current_val[size - 1] - prev_val[size - 1])) {
            local_max = fabs(current_val[size - 1] - prev_val[size - 1]);
        }
        /* check the possibility of the ending */
        local_max = 0;
        for(int i = 0 ; i < areas_per_process; i++) {
            curr = fabs(current_val[i] - prev_val[i]);
            if (curr > local_max) {
                local_max = curr;
            }
        }
        MPI_Allreduce(&local_max, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        if(global_max < e) {
            break;
        }
    } while(1);
    ended_values->p = local_points;
    ended_values->value = current_val;
    free(prev_val);
    free(ended_flags);
    MPI_Barrier(MPI_COMM_WORLD);
    return ended_values;
}

void init_local_points(int areas_per_process, point *local_points, point start_point, double hx, double hy, double hz,
        int rank) {
    for(int i = 0; i < areas_per_process; i++) {
        local_points[i].x = start_point.x + (rank * areas_per_process + i) * hx;
        local_points[i].y = start_point.y + hy;
        local_points[i].z = start_point.z + hz;
    }
}

double get_next_approx(double coeff, const double *val, double hx, double hy, double hz, point point, double a,
        val_func p) {
    return coeff * ((*(val - 1) + *(val + 1)) / (hx * hx) + 2 * *val / (hy * hy) + 2 * *val / (hz * hz) - p(point, a));
}