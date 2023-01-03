#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#ifdef __x86_64__
#include <x86intrin.h>
#endif

#ifndef SLEEP_TIME
#define SLEEP_TIME 1
#endif

#ifndef CYCLE_NUM

#ifdef __arm__
#define CYCLE_NUM 10000
#else
#define CYCLE_NUM 10000000
#endif

#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE (12 * 1024 * 1024 * 10)  // 10 x LLC
#endif

#ifdef __arm__
static inline unsigned long long __ccnt() {
  unsigned long long cycles = 0;
  __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cycles));
  return cycles;
}
#endif

int main() {
    // prepare
    unsigned long long start = 0;
    unsigned long long end = 0;

    int* array = malloc(ARRAY_SIZE * sizeof(int));
    for(size_t i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % ARRAY_SIZE;
    }

    // test
    int k = 0;
    sleep(SLEEP_TIME);
#ifndef __arm__
    start = __rdtsc();  // x86
#else
    start = __ccnt();   // arm
#endif
    for(size_t i = 0; i < CYCLE_NUM; i++) {
        k = array[k];
#include "nops" // file with asm("nop");
    }
#ifndef __arm__
    end = __rdtsc();    // x86
#else
    end = __ccnt();     // arm
#endif

    // save results
    printf("%llu", (end - start) / CYCLE_NUM);
    return 0;
}
