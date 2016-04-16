#include "shared_object.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstdio>

#include "inter_mutex.h"
#include "better_mutex.h"

using namespace boost::interprocess;

#include "timer.h"

inter_mutex m("/rmhregion");

int main() {
	Timer t;

	//Remove shared memory on destruction
	struct shm_remove {
		~shm_remove() {
			shared_memory_object::remove("MySharedMemory");
		}
	} remover;

	//Open the shared memory object.
	shared_memory_object shm(open_only                    //only create
			, "MySharedMemory"              //name
			, read_write  //read-write mode
			);

	//Map the whole shared memory in this process
	mapped_region region(shm                       //What to map
			, read_write //Map it as read-write
			);

	//Get the address of the mapped region
	void * addr = region.get_address();

	//Construct the shared structure in memory
	shared_object * data = static_cast<shared_object*>(addr);
	data->mutex.lock();
	data->ready_b = true;
	data->mutex.unlock();
	while (1) {
		scoped_lock<interprocess_mutex> lock(data->mutex);
		if (data->ready_a)
			break;
	}
	t.start();
	for (int i = 0; i < TRIALS; ++i) {
		m.lock();
		//data->mutex.lock();
		++data->shared_int;
		//data->mutex.unlock();
		m.unlock();
	}
	t.stop();
	data->end_b = true;

	//Wait until the other process ends
	while (1) {
		scoped_lock < interprocess_mutex > lock(data->mutex);
		if (data->end_a)
			break;
	}
	std::cout << "Process2 finished. Took " << t.usAvg() << "us." << std::endl;
	return 0;
}
