#include "thread.h"
#include "pcb.h"

void signalll0() {
	/*
	PCB::locked = 1;
	cout << "SIG 0 " << PCB::running->id << endl;
	asm cli;
	PCB::locked = 0;
	*/
	PCB::running->setState(PCB::FINISHED);
	PCB::running->removeFromThreadList(PCB::running->id);

	// SIGNALS //

	PCB::locked = 1;
	cout << "RUNNING SIG 1/2 (RL) THRD " << PCB::running->id << endl;
	PCB::running->parent->signal(1);
	PCB::running->signal(2); // To itself
	asm cli;
	PCB::locked = 0;

	PCB::running->freeSem();

	dispatch();
}

Thread::Thread(StackSize stackSize, Time timeSlice) {
	PCB::locked = 1;
	myPCB = new PCB(this, stackSize, timeSlice);

	// SIGNALS // CHANGED 8.7.2019.
	//myPCB->initSigArray();
	//registerHandler(0, signal0);

	PCB::locked = 0;
}

void Thread::start() {
	if (myPCB->getState() == PCB::NEW) {
		myPCB->initPCB();
		myPCB->setState(PCB::READY);
		Scheduler::put(myPCB);
	}
}

void dispatch() {
	PCB::reqContextSwitch = 1;
	PCB::timer();
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
	//PCB::locked = 1;

	// Pass if thread is finished
	if (myPCB->getState() == PCB::FINISHED) {
		//PCB::locked = 0;
		return;
	}

	// No thread can wait on the mainThread
	if (this == PCB::mainThread) {
		//PCB::locked = 0;
		return;
	}

	// No thread can wait on itself
	if (myPCB == PCB::running) {
		//PCB::locked = 0;
		return;
	}

	// No thread can wait on idleThread
	if(this == PCB::idle){
		//PCB::locked = 0;
		return;
	}

	myPCB->sem->wait(0);
	//PCB::locked = 0;
}

Thread::~Thread() {
	//waitToComplete();
	PCB::locked = 1;
	delete myPCB;
	PCB::locked = 0;
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
