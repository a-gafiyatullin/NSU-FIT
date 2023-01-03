#define comp_type float

#define Nx	9000
#define Ny	9000
#define Nt	111

#define Xa 	(comp_type)0.0
#define Xb	(comp_type)4.0
#define Ya	(comp_type)0.0
#define Yb 	(comp_type)4.0

#define hx (comp_type)((Xb - Xa) / (Nx - 1))
#define hy (comp_type)((Yb - Ya) / (Ny - 1))
#define coeff1 ((comp_type)0.2 / ((comp_type)1.0 / (hx * hx) + (comp_type)1.0 / (hy * hy)))
#define coeff2 ((comp_type)0.5 * ((comp_type)5.0 / (hx * hx) - (comp_type)1.0 / (hy * hy)))
#define coeff3 ((comp_type)0.25 * ((comp_type)1.0 / (hx * hx) + (comp_type)1.0 / (hy * hy)))
#define X(j) (Xa + (j) * hx)
#define Y(i) (Ya + (i) * hy)

#define Xs1 (Xa + (Xb - Xa) / (comp_type)3.0)
#define Xs2 (Xa + (Xb - Xa) * (comp_type)2.0 / (comp_type)3.0)
#define Ys1 (Ya + (Yb - Ya) * (comp_type)2.0 / (comp_type)3.0)
#define Ys2 (Ya + (Yb - Ya) / (comp_type)3.0)
#define R ((comp_type)0.1 * ((Xb - Xa) > (Yb - Ya) ? (Yb - Ya) : (Xb - Xa)))

#define GRID_SIZE (Nx * Ny)
#define TIME_LAYERS 2

#define get3(F, n, i, j)	F[(n) * GRID_SIZE + (i) * Nx + (j)]
#define get2(p, i, j)		p[(i) * Nx + (j)]

#define max(a,b) ((a) > (b) ? (a) : (b))

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main() {
	/* allocate memory */
	comp_type* F = malloc(TIME_LAYERS * GRID_SIZE * sizeof(comp_type));
	comp_type *p = malloc(GRID_SIZE * sizeof(comp_type));
	if(!F || !p) {
		perror("malloc");
		exit(errno);
	}

	/* init arrays */
	for(int i = 0; i < GRID_SIZE; i++) {
		F[i] = (comp_type)0.0;
	}
	for(int i = 0; i < Ny; i++) {
		for(int j = 0; j < Nx; j++) {
			if((X(j) - Xs1) * (X(j) - Xs1) + (Y(i) - Ys1) * (Y(i) - Ys1) < R * R) {
				get2(p, i, j) = (comp_type)0.1;
			} else if((X(j) - Xs2) * (X(j) - Xs2) + (Y(i) - Ys2) * (Y(i) - Ys2) < R * R) {
				get2(p, i, j) = (comp_type)-0.1;
			} else {
				get2(p, i, j) = (comp_type)0.0;
			}
		}
	}

	/* compute process */
	time_t start_time = time(NULL);

	comp_type delta = 0;
	for(int n = 0; n < Nt - 1; n++) {
		delta = 0;
		for(int i = 1; i < Ny - 1; i++) {
			for(int j = 1; j < Nx - 1; j++) {
				int n_idx = n % 2;
				int nplus1_idx = (n + 1) % 2;
				get3(F, nplus1_idx, i, j) = coeff1 * (
						coeff2 * (get3(F, n_idx, i, j - 1) + get3(F, n_idx, i, j + 1))	+
						coeff2 * (get3(F, n_idx, i - 1, j) + get3(F, n_idx, i + 1, j)) 	+
						coeff3 * (get3(F, n_idx, i - 1, j - 1) 		+
								get3(F, n_idx,  i - 1, j + 1) 	+
								get3(F, n_idx, i + 1, j - 1) 	+
								get3(F, n_idx, i + 1, j + 1)) 	+
						2.0f * get2(p, i, j)			+
						0.25f * (get2(p, i - 1, j) 		+
								get2(p, i + 1, j) 	+
								get2(p, i, j - 1) 	+
								get2(p, i, j + 1)));
				delta = max(delta, fabs(get3(F, n_idx, i, j) - get3(F, nplus1_idx, i, j)));
			}
		}
	}
	time_t end_time = time(NULL);
	printf("n = %d, sigma = %.8f\n", Nt - 1, delta);
	printf("Total time: %lld sec.\n", end_time - start_time);

	/* generate plot file */
	printf("Generating plot...\n");
	int plot_file = open("computation.plot", O_RDWR | O_CREAT, 0755);
	if(plot_file < 0) {
		perror("open");
		exit(errno);
	}
	write(plot_file, F + ((Nt - 1) % 2) * GRID_SIZE, GRID_SIZE * sizeof(comp_type));
	close(plot_file);

	execl("/usr/bin/gnuplot", "/usr/bin/gnuplot", "plot_script", NULL);
}
