#pragma once

#include <algorithm>
#include <functional>
#include <mpi.h>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

typedef std::function<bool(const std::pair<int, int>, const std::pair<int, int>)> comp_type;

class Synchro {
private:
	enum MessageType {
		REQUIRE,
		OK,
		RELEASE,
		FINISH
	};

	int max_processes_num_;
	std::vector<bool> processes_alive_;
	int rank_;
	int ok_processes_ = 1;// processes that are agreed to give lock

	std::thread thread_;
	std::mutex mutex_;

	int time_ = 0;
	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, comp_type> req_queue_;

	static bool was_init_;// synchro object had already been created
	static Synchro *synchro_;

	volatile bool allow_ = false;// allow to enter to a critical section
	// variable is volatile to prevent optimization of the endless loop in lock()

	void service_thread();
	void service_thread_stop();
	Synchro(const int &max_processes_num, const int &rank);

public:
	static Synchro *create_synchro(const int &max_processes_num, const int &rank);
	static void release_synchro();

	void lock();

	void unlock();
};
