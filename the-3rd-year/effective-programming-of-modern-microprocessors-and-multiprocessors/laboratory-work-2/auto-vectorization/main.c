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

#define get(p, i, j) p[(i) * Nx + (j)]

#define max(a, b) ((a) > (b) ? (a) : (b))

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>

int main() {
	/* allocate memory */
	float *F_low = NULL, *F_high = NULL;
	F_low = malloc(GRID_SIZE * sizeof(float));
	F_high = malloc(GRID_SIZE * sizeof(float));
	float *p = malloc(GRID_SIZE * sizeof(float));
	if(!F_low || !F_high || !p) {
		perror("malloc");
		exit(errno);
	}

	/* init arrays */
	for(int i = 0; i < Ny; i++) {
		for(int j = 0; j < Nx; j++) {
			float xj = X(j);
			float yi = Y(i);
			if((xj - Xs1) * (xj - Xs1) + (yi - Ys1) * (yi - Ys1) < R * R) {
				get(p, i, j) = 0.1f;
			} else if((xj - Xs2) * (xj - Xs2) + (yi - Ys2) * (yi - Ys2) < R * R) {
				get(p, i, j) = -0.1f;
			} else {
				get(p, i, j) = 0.0f;
			}
			get(F_low, i, j) = 0.0f;
		}
	}

	/* compute process */
	struct tms start, end;

	times(&start);
	float delta = 0.0f;
	for(int n = 0; n < Nt - 1; n++) {
		delta = 0.0f;
		float *F_curr = NULL, *F_next = NULL;
		if(n % 2 == 0) {
			F_curr = F_low;
			F_next = F_high;
		} else {
			F_next = F_low;
			F_curr = F_high;
		}
		for(int i = 1; i < Ny - 1; i++) {
			for(int j = 1; j < Nx - 1; j++) {
				float rez = coeff1 * (
						coeff3 * (get(F_curr, i - 1, j - 1) + get(F_curr,  i - 1, j + 1)) 	+
						coeff2b * (get(F_curr, i - 1, j) + get(F_curr, i + 1, j)) 			+
						coeff2 * (get(F_curr, i, j - 1) + get(F_curr, i, j + 1))			+
						coeff3 * (get(F_curr, i + 1, j - 1) + get(F_curr, i + 1, j + 1)) 	+
						0.25f * (get(p, i - 1, j)	+
						2.0f * get(p, i, j)			+
						get(p, i, j - 1) 			+
						get(p, i, j + 1))			+
						get(p, i + 1, j));
				delta = max(delta, fabs(get(F_curr, i, j) - rez));
				get(F_next, i, j) = rez;
			}
		}
	}
	times(&end);
	printf("n = %d, sigma = %.8f\n", Nt - 1, delta);
	printf("Total time: %lf sec.\n", (double)(end.tms_utime - start.tms_utime) / sysconf(_SC_CLK_TCK));

	free(F_low);
	free(F_high);
	free(p);

	return 0;
}
