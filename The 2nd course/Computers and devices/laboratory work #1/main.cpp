#include <iostream>
#include <vector>
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
        vector<int> v(10);
        for(auto &element : v)
                cin >> element;

        bubble_sort(v);

        for(auto &element : v)
                cout << element << " ";
        return 0;
}
