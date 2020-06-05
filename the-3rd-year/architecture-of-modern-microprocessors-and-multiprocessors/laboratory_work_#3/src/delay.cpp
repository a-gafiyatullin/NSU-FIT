#include <stdio.h>
#include <papi.h>
#include <vector>

constexpr unsigned long long N = 10000000;

int main() {
  int a = 0;
  int event_set = PAPI_NULL;
  long long int results = 0;

  PAPI_library_init(PAPI_VER_CURRENT);
  PAPI_create_eventset(&event_set);
  PAPI_add_event(event_set, PAPI_TOT_CYC);

  PAPI_start(event_set);
  for(long long i = 0; i < N; i++) {
      #include "branches"
  }
  PAPI_stop(event_set, &results);

  printf("%lf", (double)results / N);

  PAPI_cleanup_eventset(event_set);
  PAPI_destroy_eventset(&event_set);

  return 0;
}