#include "shared_object.h"
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>
#include <cstdio>

#include "binary_semaphore.h"
#include "interprocess_pmutex.h"
using namespace boost::interprocess;

#include "timer.h"
#include <stdlib.h>     /* atoi */

#ifdef SEMPROC
binary_semaphore m("/rmhregion");
#endif

#ifdef MUTEXPROC
interprocess_pmutex m("/rmhmutex");
#endif

int main(int argc, char** argv) {

	try {
		Timer t;

		int proc_num = 1;
		if (argc == 2) {
			proc_num = atoi(argv[1]);
		}

		//Remove shared memory on construction and destruction
		struct shm_remove {
			~shm_remove() {
				shared_memory_object::remove("MySharedMemory");
			}
		} remover;

		//Create a shared memory object.
		shared_memory_object shm(open_or_create               //only create
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
		shared_object * data = (shared_object *)region.get_address();

		//Construct the shared structure in memory
		if (proc_num == 1) {
			new ((void*) data) shared_object;
		}

		do {
			scoped_lock < interprocess_mutex > lock(data->mutex);
			++data->number_of_waiting_threads;
			if (data->number_of_waiting_threads == NUM_OF_PROCS) {
				data->number_of_waiting_threads = 0;
				data->threads_ready.notify_all();
			} else {
				data->threads_ready.wait(lock);
			}
		} while (false);

		t.start();
		for (int i = 0; i < TRIALS; ++i) {
#ifdef BOOSTPROC
			data->mutex.lock();
#else
			m.lock();
#endif
			++data->shared_int;
#ifdef BOOSTPROC
			data->mutex.unlock();
#else
			m.unlock();
#endif
		}
		t.stop();

		std::cout << "Process" << proc_num << " finished. Took "
				<< t.usAvg() / 1000000 << "sec." << std::endl;

		//Wait for the other processes to end
		do {
			scoped_lock < interprocess_mutex > lock(data->mutex);
			++data->number_of_waiting_threads;
			if (data->number_of_waiting_threads == NUM_OF_PROCS) {
				data->number_of_waiting_threads = 0;
				data->threads_ready.notify_all();
			} else {
				data->threads_ready.wait(lock);
			}
		} while (false);

		if (proc_num == 1) {
			if (data->shared_int == (TRIALS * NUM_OF_PROCS)) {
				std::cout << "Shared int is " << data->shared_int
						<< ".  Processes were properly synchronized."
						<< std::endl;
			} else {
				std::cout << "Shared int is " << data->shared_int
						<< ".  Processes were NOT properly synchronized."
						<< std::endl;
			}
		}
	} catch (interprocess_exception &ex) {
		std::cout << ex.what() << std::endl;
		return 1;
	}

	return 0;
}
