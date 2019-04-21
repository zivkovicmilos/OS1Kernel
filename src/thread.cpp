#include "thread.h"
#include "pcb.h"

void Thread::start() {
	// lock
	if (myPCB->getState() == PCB::NEW) {
		myPCB->setState(PCB::READY);
		myPCB->initPCB();
		Scheduler::put(myPCB);
	}
	// unlock
}

void dispatch() {
	//lock
	PCB::reqContextSwitch = true;
	timer();
	//unlock
}

