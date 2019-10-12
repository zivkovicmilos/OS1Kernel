#include "thread.h"
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	PCB::locked = 1;
	myPCB = new PCB(this, stackSize, timeSlice);
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
	asm cli;

	// Pass if thread is finished
	if (myPCB->getState() == PCB::FINISHED) {
		asm sti;
		return;
	}

	// No thread can wait on the mainThread
	if (this == PCB::mainThread) {
		asm sti;
		return;
	}

	// No thread can wait on itself
	if (myPCB == PCB::running) {
		asm sti;
		return;
	}

	// No thread can wait on idleThread
	if(this == PCB::idle){
		asm sti;
		return;
	}
	asm sti;
	myPCB->sem->wait(0);
}

Thread::~Thread() {
	waitToComplete();
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
