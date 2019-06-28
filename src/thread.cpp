#include "thread.h"
#include "pcb.h"

void signal0() {
	/*
	PCB::locked = 1;
	cout << "SIG 0 " << PCB::running->id << endl;
	asm cli;
	PCB::locked = 0;
	*/
	PCB::running->setState(PCB::FINISHED);
	PCB::running->removeFromThreadList(PCB::running->id);

	// SIGNALS //

	PCB::running->parent->signal(1);
	PCB::running->signal(2); // To itself

	PCB::running->freeSem();

	dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	// lock
	PCB::locked = 1;
	myPCB = new PCB(this, stackSize, timeSlice);

	// SIGNALS //
	myPCB->initSigArray();
	registerHandler(0, signal0);

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
		//cout<<"this is mainThread" << myPCB->id << endl;
		//asm cli;
		//PCB::locked = 0;
		return;
	}
	// if idle
	if (myPCB == PCB::running) {
		//cout<<"sam sebe " << myPCB->id << endl;
		//asm cli;
		//PCB::locked = 0;
		return;
	}

	//PCB::running->setState(PCB::BLOCKED);
	myPCB->sem->wait(0);
	PCB::locked = 0; // ???

	//cout<<"Is waiting: " << PCB::running->id << " on " << myPCB->id<< endl;
	//asm cli;
	//PCB::locked = 0;
	//dispatch();
}

// SIGNALS //
void Thread::signal (SignalId signal) {
	myPCB->signal(signal);
}

void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	myPCB->unregisterAllHandlers(id);
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	myPCB->swap(id, hand1, hand2);
}

void Thread::blockSignal(SignalId signal) {
	myPCB->blockSignal(signal);
}
static void blockSignalGlobally(SignalId signal) {
	PCB::blockSignalGlobally(signal);
}

void Thread::unblockSignal(SignalId signal) {
	myPCB->unblockSignal(signal);
}

static void unblockSignalGlobally(SignalId signal) {
	PCB::unblockSignalGlobally(signal);
}
////////////////////////////////////////

Thread::~Thread() {
	/*
	PCB::locked = 1;
	cout<<"DELETING THREAD..." << myPCB->id << " FROM " << PCB::running->id << endl;
	asm cli;
	PCB::locked = 0;
	*/
	// Lose nesto vrti
	//waitToComplete();
	PCB::locked = 1;
	delete myPCB; // CHANGED
	PCB::locked = 0;
}
