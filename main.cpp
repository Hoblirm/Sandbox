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

	try {
		Timer t;

		//Remove shared memory on construction and destruction
		struct shm_remove {
			shm_remove() {
				shared_memory_object::remove("MySharedMemory");
			}
			~shm_remove() {
				shared_memory_object::remove("MySharedMemory");
			}
		} remover;

		//Create a shared memory object.
		shared_memory_object shm(create_only               //only create
				, "MySharedMemory"          //name
				, read_write   //read-write mode
				);

		//Set size
		shm.truncate(sizeof(shared_object));

		//Map the whole shared memory in this process
		mapped_region region(shm                       //What to map
				, read_write   //Map it as read-write
				);

		//Get the address of the mapped region
		void * addr = region.get_address();

		//Construct the shared structure in memory
		shared_object * data = new (addr) shared_object;

		std::cout << "Process1: Waiting for process 2 to start..." << std::endl;
		while (1) {
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (data->ready_b)
				break;
			usleep(10000);
		}
		data->mutex.lock();
		data->ready_a = true;
		data->mutex.unlock();
		t.start();
		for (int i = 0; i < TRIALS; ++i) {
			m.lock();
			//data->mutex.lock();
			++data->shared_int;
			//data->mutex.unlock();
			m.unlock();
		}
		t.stop();
		data->end_a = true;

		//Wait until the other process ends
		while (1) {
			scoped_lock<interprocess_mutex> lock(data->mutex);
			if (data->end_b)
				break;
		}
		if (data->shared_int == (TRIALS * 2)) {
			std::cout << "Shared int is " << data->shared_int
					<< ".  Processes were properly synchronized." << std::endl;
		} else {
			std::cout << "Shared int is " << data->shared_int
					<< ".  Processes were NOT properly synchronized."
					<< std::endl;
		}
		std::cout << "Process1 finished. Took " << t.usAvg() << "us."
				<< std::endl;
	} catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
