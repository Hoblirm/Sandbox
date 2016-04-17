#ifndef INTERPROCESS_PMUTEX_H
#define INTERPROCESS_PMUTEX_H

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdexcept>

class interprocess_pmutex {
public:
	inline interprocess_pmutex(const char* name) {
		snprintf(name_, 128, "%s", name);

		//First we attempt to create a shared memory object if it doesn't exist.
		mode_t mode = S_IRUSR | S_IWUSR;
		int fd = shm_open(name_, O_CREAT | O_EXCL | O_RDWR, mode);

		//If fd is less than zero, we failed to create the shared memory object.
		if (fd < 0) {
			//Creation failed.  This time, just try to open the shared memory object
			fd = shm_open(name_, O_RDWR, mode);
			if (fd < 0) {
				char err[256];
				snprintf(err, 256,
						"interprocess_pmutex: shm_open() - Failed for mutex '%s'",
						name_);
				throw std::runtime_error(err);
			}

			//Since the shared memory object already existed, we simply need to map it.
			map_mutex(fd);
		} else {
			//We created the shared memory object which did NOT previously exist.
			//Therefore, the shared memory and mutex must be initialized.

			//Call ftruncate() on the shared memory object.  This simply sets
			//the size of the object in shared memory.
			if (ftruncate(fd, sizeof(pthread_mutex_t)) == -1) {
				char err[256];
				snprintf(err, 256,
						"interprocess_pmutex: ftruncate() - Failed for mutex '%s'",
						name_);
				throw std::runtime_error(err);
			}

			//Map the mutex to the shared memory object
			map_mutex(fd);

			// Initialize the mutex in shared memory
			pthread_mutexattr_t mutexattr;
			pthread_mutexattr_init(&mutexattr);
			pthread_mutexattr_setpshared(&mutexattr, PTHREAD_PROCESS_SHARED);
			pthread_mutex_init(mutex_, &mutexattr);
		}
	}

	inline ~interprocess_pmutex() {
		munmap((void*) mutex_, sizeof(pthread_mutex_t));
		shm_unlink(name_);
	}

	inline void lock() {
		pthread_mutex_lock(mutex_);
	}

	inline void unlock() {
		pthread_mutex_unlock(mutex_);
	}

private:
	pthread_mutex_t* mutex_;
	char name_[128];

	inline void map_mutex(int fd) {
		void* addr = mmap(NULL, sizeof(pthread_mutex_t),
		PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);

		if (addr == MAP_FAILED) {
			char err[256];
			snprintf(err, 256,
					"interprocess_pmutex: mmap() - Failed for mutex '%s'",
					name_);
			throw std::runtime_error(err);
		}
		mutex_ = (pthread_mutex_t*) addr;
	}
};

#endif /* INTERPROCESS_PMUTEX_H */
