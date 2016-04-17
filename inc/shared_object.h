#ifndef SHARED_OBJECT_H_
#define SHARED_OBJECT_H_

#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

#define TRIALS 1000000
#define NUM_OF_PROCS 8
struct shared_object {

	shared_object() :
			number_of_waiting_threads(0), shared_int(0) {
	}

	//Mutex to protect access to the queue
	boost::interprocess::interprocess_mutex mutex;
	boost::interprocess::interprocess_condition threads_ready;

	int number_of_waiting_threads;
	int shared_int;
};

#endif /* SHARED_OBJECT_H_ */
