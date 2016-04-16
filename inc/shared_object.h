#ifndef SHARED_OBJECT_H_
#define SHARED_OBJECT_H_

#include <boost/interprocess/sync/interprocess_mutex.hpp>

#define TRIALS 10000000
struct shared_object {

	shared_object() :
			shared_int(0), ready_a(false), ready_b(false), end_a(false), end_b(
					false) {
	}

	//Mutex to protect access to the queue
	boost::interprocess::interprocess_mutex mutex;

	int shared_int;
	bool ready_a;
	bool ready_b;
	bool end_a;
	bool end_b;
};

#endif /* SHARED_OBJECT_H_ */
