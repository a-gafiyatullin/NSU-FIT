#include "matrix.h"

int main(){
    FILE* input = fopen("input.txt", "r");
    int order = 0;
    fscanf(input, "%d", &order);
    struct matrix* A = create_matrix(order);
    get_matrix(A, input);
    print_matrix(invert_matrix(A, 100), stdout);
    return 0;
}
