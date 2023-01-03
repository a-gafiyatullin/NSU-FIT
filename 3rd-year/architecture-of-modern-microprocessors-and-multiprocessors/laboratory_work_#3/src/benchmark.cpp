#include <stdio.h>
#include <vector>
#include <papi.h>

constexpr unsigned long long N = 10000000;

int main() {
  #include "declarations"

  std::vector<int> event_codes = { PAPI_BR_CN, PAPI_BR_MSP };
  std::vector<long long> values(event_codes.size());
  int event_set = PAPI_NULL;

  PAPI_library_init(PAPI_VER_CURRENT);
  PAPI_create_eventset(&event_set);

  for (auto code : event_codes) {
    PAPI_add_event(event_set,code);
  }

  PAPI_start(event_set);
  for (unsigned long long i = 0; i < N; i++) {
    #include "branches"
  }
  PAPI_stop(event_set, values.data());

  printf("%.8f", static_cast<float>(values[1]) / values[0] * 100);

  PAPI_cleanup_eventset(event_set);
  PAPI_destroy_eventset(&event_set);

  return 0;
}