#include "sem.h"

#include "ksem.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(this, init);
}

int Semaphore::wait(Time maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val() const{
	return myImpl->val();
}

Semaphore::~Semaphore() {
	PCB::locked = 1;
	delete myImpl;
	PCB::locked = 0;
}
