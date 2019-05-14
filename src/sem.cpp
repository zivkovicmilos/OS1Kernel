#include "sem.h"
#include "ksem.h"

Semaphore::Semaphore(int init) {
	//lock
	myImpl = new KernelSem(init);
	//unlock
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
	//lock
	delete myImpl;
	//unlock
}
