#define ALIGN 32

#define VECTOR_SIZE 8
#define SHIFT1 1
#define SHIFT2 2

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

#define get(p, i, j) p[(i) * Nx + (j)]

#define max(a, b) ((a) > (b) ? (a) : (b))

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/times.h>
#include <immintrin.h>

int main() {
	/* allocate memory */
	float *F0 = _mm_malloc(GRID_SIZE * sizeof(float), ALIGN);
	float *F1 = _mm_malloc(GRID_SIZE * sizeof(float), ALIGN);
	float *p  = _mm_malloc(GRID_SIZE * sizeof(float), ALIGN);
	if(!F0 || !F1 || !p) {
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
			get(F0, i, j) = 0.0f;
		}
	}

	/* prepare for vectorization */
	const __m256i left_shift = _mm256_set_epi32(6, 5, 4, 3, 2, 1, 0, 0);
	const __m256i right_shift = _mm256_set_epi32(7, 7, 6, 5, 4, 3, 2, 1);
	const __m256i swap_first_last = _mm256_set_epi32(0, 6, 5, 4, 3, 2, 1, 7);
	const __m256 left_blend = _mm256_set_ps(~0, ~0, ~0, ~0, ~0, ~0, ~0, 0);
	const __m256 right_blend = _mm256_set_ps(0, ~0, ~0, ~0, ~0, ~0, ~0, ~0);

	const __m256 v_coeff1 = _mm256_set1_ps(coeff1);
	const __m256 v_coeff2 = _mm256_set1_ps(coeff2);
	const __m256 v_coeff2b = _mm256_set1_ps(coeff2b);
	const __m256 v_coeff3 = _mm256_set1_ps(coeff3);
	const __m256 v_coeff4 = _mm256_set1_ps(0.25f);
	const __m256 v_coeff5 = _mm256_set1_ps(2.0f);

	/* compute process */
	struct tms start, end;

	__m256 v_delta;
	times(&start);
	for(int n = 0; n < Nt - 1; n++) {
		v_delta = _mm256_setzero_ps();
		float *curr_F0;	// current time layer
		float *curr_F1;	// next time layer
		
		if(n % 2 == 0) {
			curr_F0 = F0;
			curr_F1 = F1;
		} else {
			curr_F0 = F1;
			curr_F1 = F0;
		}

		__m256 *v_F_prev = (__m256*)(curr_F0);
		__m256 *v_F_curr = (__m256*)(curr_F0 + Nx);
		__m256 *v_F_next = (__m256*)(curr_F0 + 2 * Nx);

		__m256 *v_F_rez = (__m256*)(curr_F1 + Nx);

		__m256 *v_p_prev = (__m256*)(p);
		__m256 *v_p_curr = (__m256*)(p + Nx);
		__m256 *v_p_next = (__m256*)(p + 2 * Nx);

		for(int i = 1; i < Ny - 1; i++) {

			__m256 v_F_prev_before = v_F_prev[0];
			__m256 v_F_prev_central = v_F_prev[1];
			__m256 v_F_prev_next = v_F_prev[2];

			__m256 v_F_curr_before = v_F_curr[0];
			__m256 v_F_curr_central = v_F_curr[1];
			__m256 v_F_curr_next = v_F_curr[2];

			__m256 v_F_next_before = v_F_next[0];
			__m256 v_F_next_central = v_F_next[1];
			__m256 v_F_next_next = v_F_next[2];

			__m256 v_p_curr_before = v_p_curr[0];
			__m256 v_p_curr_central = v_p_curr[1];
			__m256 v_p_curr_next = v_p_curr[2];

			// main cycle
			for(int j = 1; j < Nx / VECTOR_SIZE; j++) {
				
				__m256 v_F_prev_left = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_prev_before, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_prev_central, left_shift), left_blend);
				__m256 v_F_prev_right = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_prev_next, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_prev_central, right_shift), right_blend);

				__m256 v_F_curr_left = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_curr_before, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_curr_central, left_shift), left_blend);
				__m256 v_F_curr_right = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_curr_next, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_curr_central, right_shift), right_blend);

				__m256 v_F_next_left = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_next_before, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_next_central, left_shift), left_blend);
				__m256 v_F_next_right = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_F_next_next, swap_first_last), 
					_mm256_permutevar8x32_ps(v_F_next_central, right_shift), right_blend);

				__m256 v_p_curr_left = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_p_curr_before, swap_first_last), 
					_mm256_permutevar8x32_ps(v_p_curr_central, left_shift), left_blend);
				__m256 v_p_curr_right = _mm256_blendv_ps(_mm256_permutevar8x32_ps(v_p_curr_next, swap_first_last), 
					_mm256_permutevar8x32_ps(v_p_curr_central, right_shift), right_blend);

				__m256 rez = v_coeff1 * (
						v_coeff3 * (v_F_prev_left + v_F_prev_right)			+
						v_coeff2b * (v_F_prev_central + v_F_next_central) 	+
						v_coeff2 * (v_F_curr_left + v_F_curr_right)			+
						v_coeff3 * (v_F_next_left + v_F_next_right) 		+

						v_coeff5 * v_p_curr_central	+
						v_coeff4 * (v_p_prev[j]		+
								v_p_curr_left 		+
								v_p_curr_right		+
								v_p_next[j]));

				__m256 local_delta = _mm256_max_ps(v_F_curr_central - rez, rez - v_F_curr_central);
				v_delta = _mm256_max_ps(v_delta, local_delta);
				v_F_rez[j] = rez;

				v_F_prev_before = v_F_prev_central;
				v_F_prev_central = v_F_prev_next;

				v_F_curr_before = v_F_curr_central;
				v_F_curr_central = v_F_curr_next;

				v_F_next_before = v_F_next_central;
				v_F_next_central = v_F_next_next;

				v_p_curr_before = v_p_curr_central;
				v_p_curr_central = v_p_curr_next;

				v_F_prev_next = v_F_prev[j + 2];
				v_F_curr_next = v_F_curr[j + 2];
				v_F_next_next = v_F_next[j + 2];
				v_p_curr_next = v_p_curr[j + 2];
			}

			v_F_prev = v_F_curr;
			v_F_curr = v_F_next;
			v_F_next += Nx / VECTOR_SIZE;

			v_F_rez += Nx / VECTOR_SIZE;;

			v_p_prev = v_p_curr;
			v_p_curr = v_p_next;
			v_p_next += Nx / VECTOR_SIZE;;
		}
	}
	times(&end);

	/* print max delta */
	float max_delta = 0.0f;
	float *vec_delta = (float*)(&v_delta);
	for(int i = 0; i < VECTOR_SIZE; i++) {
		max_delta = max(max_delta, vec_delta[i]);
	}
	printf("n = %d, sigma = %.8f\n", Nt - 1, max_delta);
	printf("Total time: %lf sec.\n", (double)(end.tms_utime - start.tms_utime) / sysconf(_SC_CLK_TCK));

	_mm_free(F0);
	_mm_free(F1);
	_mm_free(p);

	return 0;
}
