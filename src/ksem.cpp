#include "ksem.h"

SemList* KernelSem::semList = new SemList();

KernelSem::KernelSem(Semaphore* sm, int init) {
	PCB::locked = 1;
	value = init;
	mySem = sm;
	limitedWait = new WaitList();
	// add limitedWait to global list
	semList->add(mySem);
	unlimitedWait = new Queue();
	PCB::locked = 0;
}

int KernelSem::val() const {
	return value;
}

int KernelSem::signal(int n) {
	if (n<0) return n; // Exception
	PCB::locked = 1;
	PCB* temp = 0;
	int ret = 0;
	int numToFree = 0;

	if (n != 0) {
		if (n >= unlimitedWait->getSize()) {
			numToFree = unlimitedWait->getSize();
		} else {
			numToFree = n;
		}
		ret = numToFree;
		for (int i = 0; i<numToFree; i++) {
			temp = unlimitedWait->getElem();
			temp->setState(PCB::READY);
			Scheduler::put(temp);
		}
		value+=n;
	} else {
		if (++value <= 0) {
			PCB* temp = unlimitedWait->getElem();
			temp->setState(PCB::READY);
			Scheduler::put(temp);
		}
		ret = 0;
	}

	PCB::locked = 0;
	return ret;
}

int KernelSem::wait(Time maxTimeToWait) {
	PCB::locked = 1;
	if (maxTimeToWait == 0) {
		if (--value < 0) {
			PCB::running->setState(PCB::BLOCKED);
			unlimitedWait->addElem(PCB::running);
			PCB::locked = 0;
			dispatch();
		}
		return 1;
	} else {
		PCB::running->setState(PCB::BLOCKED);
		limitedWait->addWaiting(PCB::running->myThread, maxTimeToWait);
		PCB::locked = 0;
		dispatch();
	}
	return 0; // Unblocked because of the timer
}

KernelSem::~KernelSem() {
	PCB::locked = 1;
	delete limitedWait;
	delete unlimitedWait;
	PCB::locked = 0;
}
