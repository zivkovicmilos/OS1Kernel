#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) {
	// lock
	myPCB = new PCB(this, stackSize, timeSlice);
	// unlock
}

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
	PCB::reqContextSwitch = 1;
	PCB::timer();
	//unlock
}

ID Thread::getId() {
	return myPCB->id;
}

ID Thread::getRunningId() {
	return PCB::running->id;
}

Thread* Thread::getThreadById(ID id) {
	return PCB::findThread(id);
}
