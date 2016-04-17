#ifndef BINARY_SEMAPHORE_H
#define BINARY_SEMAPHORE_H

#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>

class binary_semaphore {
public:
	inline binary_semaphore(const char* name) {
		snprintf(name_,128, "%s", name);
		semaphore_ = sem_open(name_, O_CREAT, S_IRUSR | S_IWUSR, 1);
	}

	inline ~binary_semaphore() {
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
	char name_[128];
};

#endif /* BINARY_SEMAPHORE_H */
