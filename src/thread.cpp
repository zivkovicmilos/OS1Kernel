#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	// lock
	PCB::locked = 1;
	myPCB = new PCB(this, stackSize, timeSlice);
	PCB::locked = 0;
	// unlock
}

void Thread::start() {
	// lock ?
	//PCB::locked = 1;
	if (myPCB->getState() == PCB::NEW) {
		myPCB->initPCB();
		myPCB->setState(PCB::READY);
		Scheduler::put(myPCB);
	}
	//PCB::locked = 0;
	//if (PCB::reqContextSwitch) dispatch();
	// unlock ?
}

void dispatch() {
	//lock ?
	PCB::reqContextSwitch = 1;
	PCB::timer();
	//unlock ?
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

void Thread::waitToComplete() {
	PCB::locked = 1;

	if (myPCB->getState() == PCB::FINISHED) {
		PCB::locked = 0;
		return;
	}

	if (this == PCB::mainThread) {
		cout<<"this is mainThread" << myPCB->id << endl;
		asm cli;
		PCB::locked = 0; return;
	}
	// if idle
	if (myPCB == PCB::running) {
		cout<<"sam sebe " << myPCB->id << endl;
		asm cli;
		PCB::locked = 0;
		return; }

	//PCB::running->setState(PCB::BLOCKED); CHANGED
	myPCB->sem->wait(0);
	PCB::locked = 0; // ???
	//cout<<"Is waiting: " << PCB::running->id << " on " << myPCB->id<< endl;
	//asm cli;
	//PCB::locked = 0;
	//dispatch();
}

Thread::~Thread() {
	/*
	PCB::locked = 1;
	cout<<"DELETING THREAD..." << myPCB->id << " FROM " << PCB::running->id << endl;
	asm cli;
	PCB::locked = 0;
	*/
	// Lose nesto vrti
	waitToComplete();
	//delete myPCB;
}
