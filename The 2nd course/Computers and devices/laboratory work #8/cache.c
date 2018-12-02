#include <stdio.h>  //for printf
#include <stdlib.h> //for malloc
#include <time.h>   //for time
#include <memory.h> //for memset

#define SINGLE_CORE_LEVEL1_CACHE_SIZE 32    //KB
#define SINGLE_CORE_LEVEL2_CACHE_SIZE 256   //KB
#define LEVEL_3_CACHE_SIZE 3072 //KB
#define MAX_SIZE 10 * 1024 * 1024 / 4 //10 MB of int
#define CYCLES_AMOUNT 100

typedef unsigned long long ull;

inline ull rdtsc();

ull rdtsc(){
    unsigned int lo, hi;
    asm volatile("rdtsc\n" : "=a"(lo), "=d"(hi));
    return ((ull)hi << 32) | lo;
}

void init_direct_bypass(int *array, int size){
    for(int i = 0; i < size; i++)
        array[i] = (i + 1) % size;
}

void init_reverse_bypass(int *array, int size){
    for(int i = size - 1; i > 0; i--)
        array[i] = (i - 1) % size;
    array[0] = size - 1;
}

void init_random_bypass(int *array, int size){
    memset(array, -1, sizeof(int) * size);
    int current_elem = 0;
    for(int i = 0; i < size - 1; i++){
        int j = rand() % size;
        while(array[j] != -1 || current_elem == j)  //search for unused array element
            j = rand() % size;
        array[current_elem] = j;
        current_elem = j;
    }
    array[current_elem] = 0;
}

ull test_access_speed(volatile int *array, int size){
    for(int j = 0, k = 0; j < size; j++)
        k = array[k];
    size *= CYCLES_AMOUNT;
    ull start = rdtsc();
    for(int j = 0, k = 0; j < size; j++)
        k = array[k];
    ull end = rdtsc();
    return  end - start;
}

int main(){
    int current_size = 256;   // 1 KB
    int *test_array;
    int step = 256;
    ull ticks_amount;
    for(; current_size < MAX_SIZE; current_size += step){
        srand(time(NULL));
        test_array = malloc(sizeof(int) * current_size);
        //test direct access
        init_direct_bypass(test_array, current_size);
        ticks_amount = test_access_speed(test_array, current_size);
        printf("Direct bypass, array size: %d, amount of ticks: %llu\n", current_size,
               ticks_amount / (current_size * CYCLES_AMOUNT));
        //test reverse access
        init_reverse_bypass(test_array, current_size);
        ticks_amount = test_access_speed(test_array, current_size);
        printf("Reverse bypass, array size: %d, amount of ticks: %llu\n", current_size,
               ticks_amount / (current_size * CYCLES_AMOUNT));
        //test random access
        init_random_bypass(test_array, current_size);
        ticks_amount = test_access_speed(test_array, current_size);
        printf("Random bypass, array size: %d, amount of ticks: %llu\n", current_size,
                ticks_amount / (current_size * CYCLES_AMOUNT));
        printf("-------------------------------------------------------------------------------\n");
        if(current_size == SINGLE_CORE_LEVEL1_CACHE_SIZE * 1024 / 4){
            step *= SINGLE_CORE_LEVEL2_CACHE_SIZE / SINGLE_CORE_LEVEL1_CACHE_SIZE;
            printf("LEVEL 2:\n");
        }
        else if(current_size == SINGLE_CORE_LEVEL2_CACHE_SIZE * 1024 / 4){
            step *= LEVEL_3_CACHE_SIZE / SINGLE_CORE_LEVEL2_CACHE_SIZE;
            printf("LEVEL 3:\n");
        }
        free(test_array);
    }
    return 0;
}
