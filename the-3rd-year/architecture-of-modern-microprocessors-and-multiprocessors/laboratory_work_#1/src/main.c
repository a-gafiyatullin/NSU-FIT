#include <stdio.h>
#include <time.h>
#include <x86intrin.h>

#ifndef WARMING_UP_SECONDS
#define WARMING_UP_SECONDS 10
#endif

#ifndef CYCLE_NUM
#define CYCLE_NUM 100000000
#endif

int main() {
  unsigned long long start = 0;
  unsigned long long end = 0;

  unsigned x = 1 << 31;
  unsigned y = 2;
  unsigned result = 0;

  double clocks_per_empty_iter = 0;

  // warming up a processor
  time_t start_time = time(NULL);
  while (time(NULL) - start_time < WARMING_UP_SECONDS) {
    result = x / y;
  }
  printf("Processor is warmed up! result = %d\n", result);

  // count cycle clocks
  int i;
  start = __rdtsc();
  for (i = 0; i < CYCLE_NUM; i++) {
  }
  end = __rdtsc();
  clocks_per_empty_iter = (double)(end - start) / i;
  printf("Amount of clocks per empty iteration: %lf\n", clocks_per_empty_iter);

  //---------------------------independent operations---------------------------
  // count clocks for cycle with divisions
  start = __rdtsc();
  for (i = 0; i < CYCLE_NUM; i++) {
    result = x / y;
  }
  end = __rdtsc();
  printf("Amount of clocks per iteration with division: %lf, result = %d\n\n",
         (double)(end - start) / i, result);

  // real division clocks
  printf("Division clocks for independent operations: %lf\n\n",
         (double)(end - start) / i - clocks_per_empty_iter);
  //---------------------------Dependent operations-----------------------------
  x = 1 << 31;

  start = __rdtsc();
  for (i = 0; i < CYCLE_NUM; i++) {
    x = x / y;
  }
  end = __rdtsc();
  printf("Amount of clocks per iteration with division: %lf, result = %d\n\n",
         (double)(end - start) / i, x);

  // real division clocks
  printf("Division clocks for dependent operations: %lf\n\n",
         (double)(end - start) / i - clocks_per_empty_iter);

  return 0;
}