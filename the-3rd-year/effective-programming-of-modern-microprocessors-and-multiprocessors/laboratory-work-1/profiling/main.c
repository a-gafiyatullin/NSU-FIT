#define Nx	9000
#define Ny	9000
#define Nt	111

#define Xa 	0.0f
#define Xb	4.0f
#define Ya	0.0f
#define Yb 	4.0f

#define hx ((Xb - Xa) / (Nx - 1))
#define hy ((Yb - Ya) / (Ny - 1))
#define coeff1 (0.2f / ((1.0f / (hx * hx) + 1.0f / (hy * hy))))
#define coeff2 (0.5f * (5.0f / (hx * hx) - 1.0f / (hy * hy)))
#define coeff2b (0.5f * (5.0f / (hy * hy) - 1.0f / (hx * hx)))
#define coeff3 (0.25f * (1.0f / (hx * hx) + 1.0f / (hy * hy)))
#define X(j) (Xa + (j) * hx)
#define Y(i) (Ya + (i) * hy)

#define Xs1 (Xa + (Xb - Xa) / 3.0f)
#define Xs2 (Xa + (Xb - Xa) * 2.0f / 3.0f)
#define Ys1 (Ya + (Yb - Ya) * 2.0f / 3.0f)
#define Ys2 (Ya + (Yb - Ya) / 3.0f)
#define R (0.1f * ((Xb - Xa) > (Yb - Ya) ? (Yb - Ya) : (Xb - Xa)))

#define GRID_SIZE (Nx * Ny)
#define TIME_LAYERS 2

#define get3(F, n, i, j)	F[(n) * GRID_SIZE + (i) * Nx + (j)]
#define get2(p, i, j)		p[(i) * Nx + (j)]

#define max(a, b) ((a) > (b) ? (a) : (b))

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

void init_arrays(float *F, float  *p);
float compute_process(float *F, float *p);

int main() {
	/* allocate memory */
	float* F = malloc(TIME_LAYERS * GRID_SIZE * sizeof(float));
	float *p = malloc(GRID_SIZE * sizeof(float));
	if(!F || !p) {
		perror("malloc");
		exit(errno);
	}

	init_arrays(F, p);
	time_t start_time = time(NULL);
	float delta = compute_process(F, p);
	time_t end_time = time(NULL);

	printf("n = %d, sigma = %.8f\n", Nt - 1, delta);
	printf("Total time: %ld sec.\n", end_time - start_time);
}

void init_arrays(float *F, float  *p) {
	for(int i = 0; i < Ny; i++) {
		for(int j = 0; j < Nx; j++) {
			float xj = X(j);
			float yi = Y(i);
			if((xj - Xs1) * (xj - Xs1) + (yi - Ys1) * (yi - Ys1) < R * R) {
				get2(p, i, j) = 0.1f;
			} else if((xj - Xs2) * (xj - Xs2) + (yi - Ys2) * (yi - Ys2) < R * R) {
				get2(p, i, j) = -0.1f;
			} else {
				get2(p, i, j) = 0.0f;
			}
			get2(F, i, j) = 0.0f;
		}
	}
}

float compute_process(float *F, float *p) {
	float delta = 0;
	for(int n = 0; n < Nt - 1; n++) {
		int n_idx = n % 2;
		int nplus1_idx = (n + 1) % 2;

		delta = 0;
		for(int i = 1; i < Ny - 1; i++) {
			float f_left_down_cell = get3(F, n_idx, i - 1, 0);
			float f_current_down_cell = get3(F, n_idx, i - 1, 1);
			float f_right_down_cell = get3(F, n_idx,  i - 1, 2);

			float f_left_current_cell = get3(F, n_idx, i, 0);
			float f_current_current_cell = get3(F, n_idx, i, 1);
			float f_right_current_cell = get3(F, n_idx, i, 2);

			float f_left_up_cell = get3(F, n_idx, i + 1, 0);
			float f_current_up_cell = get3(F, n_idx, i + 1, 1);
			float f_right_up_cell = get3(F, n_idx, i + 1, 2);

			float p_left_current_cell = get2(p, i, 0);
			float p_current_current_cell = get2(p, i, 1);
			float p_right_current_cell = get2(p, i, 2);

			for(register int j = 1; j < Nx - 1; j++) {
				float rez = coeff1 * (
						coeff2b * (f_current_down_cell + f_current_up_cell) 					+
						coeff3 * (f_left_up_cell + f_right_up_cell + f_left_down_cell + f_right_down_cell)	+
						coeff2 * (f_left_current_cell + f_right_current_cell)					+
						0.25f * (
						2.0f * p_current_current_cell		+
						p_left_current_cell 			+
						p_right_current_cell)			+
						0.25f * get2(p, i - 1, j) 		+
						get2(p, i + 1, j));

				get3(F, nplus1_idx, i, j) = rez;
				delta = max(delta, fabs(f_current_current_cell - rez));

				f_left_down_cell = f_current_down_cell;
				f_current_down_cell = f_right_down_cell;
				f_left_up_cell = f_current_up_cell;
				f_current_up_cell = f_right_up_cell;
				f_left_current_cell = f_current_current_cell;
				f_current_current_cell = f_right_current_cell;
				f_right_down_cell = get3(F, n_idx,  i - 1, j + 2);
				f_right_current_cell = get3(F, n_idx,  i, j + 2);
				f_right_up_cell = get3(F, n_idx, i + 1, j + 2);

				p_left_current_cell = p_current_current_cell;
				p_current_current_cell = p_right_current_cell;
				p_right_current_cell = get2(p, i, j + 2);
			}
		}
	}

	return (delta);
}
