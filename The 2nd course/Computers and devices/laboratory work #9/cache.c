#include <stdio.h>  //for printf
#include <stdlib.h> //for malloc

#define LEVEL_3_CACHE_SIZE 3072 //KB
#define OFFSET (LEVEL_3_CACHE_SIZE * 1024 / 4)
#define SEGMENTS_AMOUNT 32

typedef unsigned long long ull;

inline ull rdtsc();

ull rdtsc(){
    unsigned int lo, hi;
    asm volatile("rdtsc\n" : "=a"(lo), "=d"(hi));
    return ((ull)hi << 32) | lo;
}

int main(){
    int bypass_size = SEGMENTS_AMOUNT * OFFSET;
    volatile int *cache;
    for(int size = OFFSET; size <= SEGMENTS_AMOUNT * OFFSET; size += OFFSET){
        cache = malloc(size * sizeof(int));
        for(int j = 0; j < size; j += (j % OFFSET < OFFSET / (size / OFFSET) ? 1 : OFFSET - (j % OFFSET)))
            cache[j] = (j + OFFSET) % size;
        ull start = rdtsc();
        for(int j = 0, k = 0; j < bypass_size; j++)
            k = cache[k];
        ull end = rdtsc();
        printf("SEGMENTS AMOUNT: %d, PROC. CLOCKS: %llu\n", size / OFFSET, (end - start) / bypass_size);
        free(cache);
    }
}