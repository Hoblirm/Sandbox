#ifndef MUTEX_H
#define MUTEX_H

#include <semaphore.h>
#include <string.h>

class inter_mutex {
public:
	inline inter_mutex(const char* name) {
		semaphore_ = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, 1);
		strcpy(name_, name);
	}

	inline ~inter_mutex() {
		sem_close(semaphore_);
		sem_unlink(name_);
	}

	inline void lock() {
		sem_wait(semaphore_);
	}

	inline void unlock() {
		sem_post(semaphore_);
	}

private:
	sem_t* semaphore_;
	char name_[64];
};

#endif /* MUTEX_H */
