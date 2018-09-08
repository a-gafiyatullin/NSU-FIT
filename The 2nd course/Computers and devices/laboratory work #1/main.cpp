#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sys/times.h>
#include <unistd.h>
using namespace std;

template<typename T>
void bubble_sort(vector<T> &v)
{
        for(auto out_iter = v.end(); out_iter != v.begin(); out_iter--)
                for(auto in_iter = v.begin(); in_iter != out_iter; in_iter++)
                        if(*in_iter > *(in_iter + 1))
                                swap(*in_iter, *(in_iter + 1));
}

int main()
{
        srand(time(NULL));
        long long int size;
        cin >> size;
        vector<int> v(size);
        for(auto &element : v)
                element = rand();

        struct tms start, finish;
        long clocks_per_sec = sysconf(_SC_CLK_TCK);
        times(&start);
        bubble_sort(v);
        times(&finish);
        double clocks = finish.tms_utime - start.tms_utime;
        cout << "Total process time: " << (double)clocks / clocks_per_sec
                << "s" << endl;

        return 0;
}
