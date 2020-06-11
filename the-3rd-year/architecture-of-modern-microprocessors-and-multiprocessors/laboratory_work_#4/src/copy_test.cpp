#include <iostream>
#include <chrono>
#include <emmintrin.h>
#include <smmintrin.h>
#include <omp.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>    // for SetThreadAffinityMask
#elif __linux__
#include <sched.h>  // for sched_setaffinity
#endif

constexpr int ALIGN = 32;

constexpr size_t MBYTES = 512; // test array volume
constexpr size_t BUFFER_SIZE = 1024 * 1024 * MBYTES;
constexpr int TEST_TRIES = 50;

double test_copy(__m128i* buffer1, __m128i* buffer2) {
    double start = omp_get_wtime();
    for (size_t i = 0; i < BUFFER_SIZE / sizeof(__m128i); i++) {
        _mm_stream_si128(buffer2 + i, _mm_stream_load_si128(buffer1 + i));     // non-temporal read-write
    }
    double end = omp_get_wtime();

    return (end - start);
}

void set_thread_affinity(int max_threads) {
#pragma omp parallel num_threads(max_threads)
    {
#ifdef _WIN32
    DWORD_PTR mask = (1 << omp_get_thread_num());
    SetThreadAffinityMask(GetCurrentThread(), mask);
#elif __linux__
    int cpu = omp_get_thread_num();
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu , &cpuset);
    sched_setaffinity(0, sizeof(cpuset), &cpuset);
#endif
    }
}

int main(int argc, char *argv[]) {
    int cores_num = 1;

    if(argc >= 2) {
        cores_num = atoi(argv[1]);
    }

    set_thread_affinity(cores_num);

    double curr_speed = 0;
    double result = 0;
#pragma omp parallel num_threads(cores_num)
        {
            for (int j = 0; j < TEST_TRIES; j++) {
#pragma omp single
                curr_speed = 0;
                __m128i* buffer1 = (__m128i*)_mm_malloc(BUFFER_SIZE, ALIGN);
                __m128i* buffer2 = (__m128i*)_mm_malloc(BUFFER_SIZE, ALIGN);

                memset(buffer1, 0, BUFFER_SIZE);
                memset(buffer2, 0, BUFFER_SIZE);
#pragma omp barrier
                test_copy(buffer1, buffer2);
                double elapsed = test_copy(buffer2, buffer1);
                test_copy(buffer1, buffer2);
#pragma omp critical
                {
                    curr_speed += MBYTES / 1024.0 / elapsed;
                }
#pragma omp barrier
#pragma omp single
                {
                    result = std::max<double>(curr_speed, result);
                }

                _mm_free(buffer1);
                _mm_free(buffer2);
            }
        }

    std::cout << result;

    return 0;
}
