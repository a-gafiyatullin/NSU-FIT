#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int size, rank, prev;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0) {
        fprintf(stderr, "I'm process #%d from %d\n", 1, size);
        MPI_Send(&rank, 1, MPI_INT, 1, rank, MPI_COMM_WORLD);
    } else if (rank != size - 1){
        MPI_Recv(&prev, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fprintf(stderr, "I'm process #%d from %d\n", rank + 1, size);
        MPI_Send(&rank, 1, MPI_INT, rank + 1, rank, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&prev, 1, MPI_INT, rank - 1, rank - 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        fprintf(stderr, "I'm process #%d from %d\n", rank + 1, size);
    }

    MPI_Finalize();
    return 0;
}