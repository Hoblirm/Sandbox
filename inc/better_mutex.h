#ifndef BETTER_MUTEX_H
#define BETTER_MUTEX_H

#include <string.h>
#include <pthread.h>

class better_mutex {
public:
	inline better_mutex(const char* name) {

		int fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
		//if (fd == -1)
		//    /* Handle error */;

		if (ftruncate(fd, sizeof(pthread_mutex_t)) == -1) {
			/* Handle error */;
		}

		mutex_ = (pthread_mutex_t*) mmap(NULL, sizeof(pthread_mutex_t),
				PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

		/* Initialise attribute to mutex. */
		pthread_mutexattr_init(&mutexattr_);
		pthread_mutexattr_setpshared(&mutexattr_, PTHREAD_PROCESS_SHARED);
		pthread_mutex_init(mutex_, &mutexattr_);
	}

	inline ~better_mutex() {
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
	char name_[64];
};


#endif /* BETTER_MUTEX_H */
