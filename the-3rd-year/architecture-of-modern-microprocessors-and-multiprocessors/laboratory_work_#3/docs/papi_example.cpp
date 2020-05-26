// Compilation: g++ -std=c++11 papi_example.cpp -lpapi
#include<cstdlib>
#include<algorithm>
#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<papi.h>
#include<x86intrin.h>

#define CHECK(x,code) {\
  int retval = x;\
  if (retval != code) {\
    std::cout << #x " : PAPI error " << retval << " : " << PAPI_strerror(retval) << std::endl;\
     exit(1);\
  }\
}

std::string event_descr(int event_code) {
  PAPI_event_info_t info;
  CHECK(PAPI_get_event_info(event_code,&info),PAPI_OK);
  //return std::string(info.symbol);      // Event name
  return std::string(info.short_descr); // Short event description
  //return std::string(info.long_descr);  // Long event description
}

int main() {

  std::vector<int> event_codes = { PAPI_TOT_CYC, PAPI_BR_CN, PAPI_BR_MSP, PAPI_BR_PRC };
  std::vector<long long> values(event_codes.size());
  int event_set = PAPI_NULL;

  CHECK(PAPI_library_init(PAPI_VER_CURRENT),PAPI_VER_CURRENT);
  CHECK(PAPI_create_eventset(&event_set),PAPI_OK);

  for (auto code : event_codes)
    CHECK(PAPI_add_event(event_set,code),PAPI_OK);

  std::cout << "        " << std::setw(11) << "rdtsc tics";
  for (auto code : event_codes)
    std::cout << std::setw(20) << event_descr(code);
  std::cout << std::setw(20) << "% of mispredict" << std::endl;

  for (size_t n = 100; n<=1000; n+=100) {

    std::vector<int> x(n);
    std::generate(x.begin(),x.end(),std::rand);

    CHECK(PAPI_start(event_set),PAPI_OK);
    long long t0 = _rdtsc();

    std::sort(x.begin(),x.end());

    long long t1 = _rdtsc();
    CHECK(PAPI_stop(event_set, values.data()),PAPI_OK);
    //CHECK(PAPI_reset(event_set),PAPI_OK);

    std::cout << std::setw(6) << n << " :" << std::setw(11) << t1-t0;
    for (auto v : values)
      std::cout << std::setw(20) << v;
    std::cout << std::setw(20) << std::setprecision(4)
              << static_cast<float>(values[2])/values[1]*100 << std::endl;
  }

  CHECK(PAPI_cleanup_eventset(event_set),PAPI_OK);
  CHECK(PAPI_destroy_eventset(&event_set),PAPI_OK);

  return 0;
}
