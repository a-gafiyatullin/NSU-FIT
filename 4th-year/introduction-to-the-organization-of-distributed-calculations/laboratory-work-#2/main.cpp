#include <iostream>
#include <mpi/mpi.h>
#include <queue>
#include <unistd.h>

enum MessageType {
	REQUIRE = 1,
	OK,
	RELEASE,
	END// process done all its job
};

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::cerr << "usage: " + std::string(argv[0]) + " [max_wait_period] [cycle_count]" << std::endl;
		return -1;
	}

	int size, rank, current_time = 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// memory to test the critical section
	MPI_Win win;
	MPI_Aint mem_size = 1;
	int *shared;
	if (rank == 0) {
		MPI_Win_allocate_shared(mem_size, sizeof(int), MPI_INFO_NULL,
					MPI_COMM_WORLD, &shared, &win);
		(*shared) = 0;
	} else {
		int disp_unit;
		MPI_Win_allocate_shared(0, sizeof(int), MPI_INFO_NULL,
					MPI_COMM_WORLD, &shared, &win);
		MPI_Win_shared_query(win, 0, &mem_size, &disp_unit, &shared);
	}
	MPI_Barrier(MPI_COMM_WORLD);

	// wait random period of time before the algorithm
	int wait_period = atoi(argv[1]);
	if (wait_period != 0) {
		srand(time(nullptr));
		int sleep_time = rand() % wait_period;
		sleep(sleep_time);
	}

	// algorithm implementation
	// comparator
	auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
		if (a.first > b.first) {
			return true;
		} else if (a.first < b.first) {
			return false;
		}
		return a.second > b.second;// pids cannot be the same
	};
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> req_queue(cmp);
	int cycle_count = atoi(argv[2]);

	// main cycle
	int alive_processes = size;
	std::vector<int> process_status(size);
	process_status[rank] = END;// exclude this process from the send list
	for (int i = 0; i < cycle_count; i++) {
		current_time++;
		req_queue.push(std::make_pair(current_time, rank));

		// notify all processes about the willingness to get the critical section
		for (int j = 0; j < size; j++) {
			if (process_status[j] != END) {
				MPI_Send(&current_time, 1, MPI_INT, j, REQUIRE, MPI_COMM_WORLD);
			}
		}
		int ok_pid_count = 1;
		// synchro cycle
		while (true) {
			if (alive_processes == 1) {
				break;
			}
			int received_clock;
			MPI_Status status;

			MPI_Recv(&received_clock, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			current_time = std::max(current_time, received_clock) + 1;
			// check the message type
			switch (status.MPI_TAG) {
				case OK:
					ok_pid_count++;
					break;
				case REQUIRE:
					req_queue.push(std::make_pair(received_clock, status.MPI_SOURCE));
					MPI_Send(&current_time, 1, MPI_INT, status.MPI_SOURCE, OK, MPI_COMM_WORLD);
					break;
				case RELEASE:
					req_queue.pop();
					break;
				case END:
					alive_processes--;
					process_status[status.MPI_SOURCE] = END;
					break;
			}

			// all processes answered and this process request is at the top of the queue
			// ok_pid_count >= alive_processes because a process can change his state from RELEASE to END
			if (ok_pid_count >= alive_processes && req_queue.top().second == rank) {
				break;
			}
		}

		// critical section
		(*shared)++;
		std::cout << "Process " << rank << " in the cycle " << i << ", shared = " << *shared << std::endl;

		// leave the critical section
		current_time++;
		req_queue.pop();
		for (int j = 0; j < size; j++) {
			if (process_status[j] != END) {
				MPI_Send(&current_time, 1, MPI_INT, j, RELEASE, MPI_COMM_WORLD);
			}
		}
	}

	// this process has done all its job
	current_time++;
	for (int j = 0; j < size; j++) {
		if (process_status[j] != END) {
			MPI_Send(&current_time, 1, MPI_INT, j, END, MPI_COMM_WORLD);
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0) {
		sleep(1);
		std::cout << "END VALUE = " << *shared << std::endl;
	}

	MPI_Win_free(&win);
	MPI_Finalize();

	return 0;
}
