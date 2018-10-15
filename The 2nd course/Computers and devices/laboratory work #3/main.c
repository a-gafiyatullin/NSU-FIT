#include <stdio.h>
#include <stdlib.h>
void swap(int *left, int *right)
{
        int tmp = *left;
        *left = *right;
        *right = tmp;
}

void bubble_sort(int *begin, int *end)
{
        for(int *out_iter = end - 1; out_iter != begin; out_iter--)
                for(int *in_iter = begin; in_iter != out_iter; in_iter++)
                        if(*in_iter > *(in_iter + 1))
                                swap(in_iter, in_iter + 1);
}

int main()
{
        int size = 0;
        scanf("%d", &size);
        int *array = (int*)malloc(size * sizeof(int));
        for(int i = 0; i < size; i++)
                scanf("%d", array + i);
        bubble_sort(array, array + size);
        for(long long i = 0; i < size; i++)
                printf("%d", array[i]);
        return 0;
}
