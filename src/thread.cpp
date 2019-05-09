#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	// lock
	PCB::lockFlag = 0;
	myPCB = new PCB(this, stackSize, timeSlice);
	PCB::lockFlag = 1;
	// unlock
}

void Thread::start() {
	// lock
	PCB::lockFlag = 0;
	if (myPCB->getState() == PCB::NEW) {
		myPCB->initPCB();
		myPCB->setState(PCB::READY);
		Scheduler::put(myPCB);
	}
	PCB::lockFlag = 1;
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
