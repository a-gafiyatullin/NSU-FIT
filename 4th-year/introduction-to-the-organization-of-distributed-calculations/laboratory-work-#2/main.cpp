#include "Synchro/Synchro.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: " + std::string(argv[0]) + " [max_wait_period] [cycle_count]" << std::endl;
		return -1;
	}

	int size, rank, provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (provided != MPI_THREAD_MULTIPLE) {
		std::cerr << "MPI don't provide MPI_THREAD_MULTIPLE" << std::endl;
		return -1;
	}
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	Synchro *synchro = Synchro::create_synchro(size, rank);

	// memory to test the critical section
	MPI_Comm node_comm;
	MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &node_comm);
	MPI_Win win;
	int mem_size = (rank == 0) ? 1 : 0;
	int *shared;
	MPI_Win_allocate_shared(mem_size, 1, MPI_INFO_NULL,
				MPI_COMM_WORLD, &shared, &win);
	if (rank == 0) {
		(*shared) = 0;
	} else {
		MPI_Aint size_data;
		int disp_unit;
		MPI_Win_shared_query(win, 0, &size_data, &disp_unit, &shared);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	// wait random period of time before the algorithm
	int wait_period = atoi(argv[1]);
	if (wait_period != 0) {
		srand(time(nullptr));
		int sleep_time = rand() % wait_period;
		sleep(sleep_time);
	}

	int cycle_count = atoi(argv[2]);

	// main cycle
	for (int i = 0; i < cycle_count; i++) {
		// critical section
		synchro->lock();

		(*shared)++;
		std::cout << "Process " << rank << " in the cycle " << i << ", shared = " << *shared << std::endl;

		synchro->unlock();
	}

	Synchro::release_synchro();

	std::cout.flush();
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank == 0) {
		std::cout << "END VALUE = " << *shared << std::endl;
	}

	MPI_Win_free(&win);
	MPI_Finalize();

	return 0;
}
