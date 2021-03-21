#include "Synchro.h"

const auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
	if (a.first > b.first) {
		return true;
	} else if (a.first < b.first) {
		return false;
	}
	return a.second > b.second;// pids cannot be the same
};

Synchro *Synchro::synchro_ = nullptr;
bool Synchro::was_init_ = false;

Synchro::Synchro(const int &max_processes_num, const int &rank)
    : max_processes_num_(max_processes_num), rank_(rank), processes_alive_(max_processes_num, true), req_queue_(cmp) {
}

Synchro *Synchro::create_synchro(const int &max_processes_num, const int &rank) {
	if (!was_init_) {
		was_init_ = true;
		synchro_ = new Synchro(max_processes_num, rank);
		synchro_->thread_ = std::thread(&Synchro::service_thread, synchro_);

		return synchro_;
	} else {
		return synchro_;
	}
}

void Synchro::release_synchro() {
	was_init_ = false;
	synchro_->service_thread_stop();

	synchro_->thread_.join();

	delete synchro_;
	synchro_ = nullptr;
}

void Synchro::service_thread() {
	int flag = 1;

	while (true) {
		// all processes answered and this process request is at the top of the queue
		// ok_pid_count >= alive_processes because a process can change his state from RELEASE to DEAD
		mutex_.lock();
		if (ok_processes_ == max_processes_num_) {
			if (!req_queue_.empty() && req_queue_.top().second == rank_) {
				allow_ = true;
			}
		}
		mutex_.unlock();

		int received_clock;
		MPI_Status status;
		MPI_Request request;

		if (flag != 0) {
			MPI_Irecv(&received_clock, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
		}

		MPI_Test(&request, &flag, &status);
		if (flag == 0) {
			continue;
		}

		mutex_.lock();
		time_ = std::max(time_, received_clock) + 1;
		mutex_.unlock();

		// check the message type
		switch (status.MPI_TAG) {
			case OK:
				mutex_.lock();
				ok_processes_++;
				mutex_.unlock();
				break;
			case REQUIRE:
				mutex_.lock();
				req_queue_.push(std::make_pair(received_clock, status.MPI_SOURCE));
				MPI_Send(&time_, 1, MPI_INT, status.MPI_SOURCE, OK, MPI_COMM_WORLD);
				mutex_.unlock();
				break;
			case RELEASE:
				mutex_.lock();
				req_queue_.pop();
				mutex_.unlock();
				break;
			case FINISH: {
				processes_alive_[status.MPI_SOURCE] = false;
				long stopped_proc = std::count(processes_alive_.begin(), processes_alive_.end(), false);
				if (stopped_proc == max_processes_num_) {
					return;
				}
				break;
			}
			default:
				continue;
		}
	}
}

void Synchro::lock() {
	mutex_.lock();

	ok_processes_ = 0;
	allow_ = false;

	time_++;
	// notify all processes about the willingness to get the critical section
	for (int j = 0; j < max_processes_num_; j++) {
		MPI_Send(&time_, 1, MPI_INT, j, REQUIRE, MPI_COMM_WORLD);
	}

	mutex_.unlock();

	// spinlock
	while (!allow_) {
	}
}

void Synchro::unlock() {
	mutex_.lock();

	time_++;

	for (int j = 0; j < max_processes_num_; j++) {
		MPI_Send(&time_, 1, MPI_INT, j, RELEASE, MPI_COMM_WORLD);
	}

	mutex_.unlock();
}

void Synchro::service_thread_stop() {
	mutex_.lock();

	time_++;
	for (int j = 0; j < max_processes_num_; j++) {
		MPI_Send(&time_, 1, MPI_INT, j, FINISH, MPI_COMM_WORLD);
	}

	mutex_.unlock();
}
