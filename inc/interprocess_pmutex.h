#ifndef INTERPROCESS_PMUTEX_H
#define INTERPROCESS_PMUTEX_H

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

class interprocess_pmutex {
public:
	inline interprocess_pmutex(const char* name) {
		snprintf(name_, 128, "%s", name);
		int fd = shm_open(name_, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		if (fd == -1) {
			/* Handle error */;
		}

		if (ftruncate(fd, sizeof(pthread_mutex_t)) == -1) {
			/* Handle error */;
		}

		void* addr = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE,
		MAP_SHARED, fd, 0);

		if (addr == MAP_FAILED) {
			/* Handle error */
		}
		mutex_ = (pthread_mutex_t*) addr;

		/* Initialize attribute to mutex. */
		pthread_mutexattr_init(&mutexattr_);
		pthread_mutexattr_setpshared(&mutexattr_, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(mutex_, &mutexattr_);
	}

	inline ~interprocess_pmutex() {
		pthread_mutex_destroy(mutex_);
		pthread_mutexattr_destroy(&mutexattr_);
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
	pthread_mutexattr_t mutexattr_;
	char name_[128];
};

#endif /* INTERPROCESS_PMUTEX_H */
