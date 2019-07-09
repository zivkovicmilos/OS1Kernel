#include "pcb.h"
#include "idleT.h"
#include "ksem.h"

unsigned int PCB::cnt = 0;
IdleThread* PCB::idle = 0;
Thread* PCB::mainThread = 0;
volatile unsigned int PCB::locked = 0;
ThreadList* PCB::threadList = new ThreadList();
PCB* PCB::running = 0;
volatile int PCB::reqContextSwitch = 0;

typedef void (*SignalHandler)();

unsigned tss;
unsigned tsp;
unsigned tbp;

void tick();

volatile int currentSlice = -1;

void PCB::signal0() {
	setState(PCB::FINISHED);
	removeFromThreadList(id);

	// SIGNALS //
	if(PCB::running->myThread == mainThread) {
		if(parent != 0) parent->signal(1);
		signal(2); // To itself
	}
	freeSem();

	//dispatch();
}


PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	PCB::locked = 1;

	id = ++cnt;
	state = NEW;
	myThread = t;

	if(id == 1 || id == 2) {
		// The main and idle threads don't have parents
		parent = 0;
	} else {
		parent = PCB::running->myThread;
	}

	sigQueue = new SigQueue();

	if (stackSize > maxStackSize) {
		this->stackSize = maxStackSize;
	} else {
		this->stackSize = stackSize;
	}
	this->timeSlice = timeSlice;
	stackPointer = stackSegment = basePointer = 0;
	threadList->add(t);
	sem = new Semaphore(0);

	initSigArray();
	//registerHandler(0, sig0);
	PCB::locked = 0;
}

Time PCB::getTimeSlice() {
	return timeSlice;
}

void PCB::wrapper() {
	PCB::running->myThread->run();

	if(PCB::running->getState() != PCB::FINISHED) {
			PCB::running->setState(FINISHED);
			threadList->remove(PCB::running->id);

			// SIGNALS //
			/*
			PCB::locked = 1;
			cout << "RUNNING SIG 1/2 WRP" << PCB::running->id << endl;

			asm cli;
			PCB::locked = 0;
			*/
			if(PCB::running->parent != 0) PCB::running->parent->signal(1);
			PCB::running->signal(2); // To itself
			// Do all this even if
			while (PCB::running->sem->val() < 0) {
				PCB::running->sem->signal(0);
			}
			dispatch();
	}
	// if the thread was FINISHED when exiting the run method, it was killed earlier
}

void (*body)() = PCB::wrapper;

void PCB::initPCB() {
	unsigned* stack = new unsigned[stackSize];
	stack[stackSize-1] = 0x200; // Set the I flag

	stack[stackSize-2] = FP_SEG(body);
	stack[stackSize-3] = FP_OFF(body);

	stackPointer = FP_OFF(stack+stackSize-12); // 12 bytes of regs are saved when entering the interrupt
	stackSegment = FP_SEG(stack+stackSize-12);
	basePointer = FP_OFF(stack+stackSize-12);
}

Thread* PCB::findThread(ID id) {
	return PCB::threadList->getByID(id);
}

PCB::threadState PCB::getState() {
	return state;
}

void PCB::setState(threadState s) {
	state = s;
}

void PCB::setSP(unsigned int sp) {
	stackPointer = sp;
}

unsigned int PCB::getSP() const {
	return stackPointer;
}

unsigned int PCB::getSS() const {
	return stackSegment;
}

unsigned int PCB::getBP() const {
	return basePointer;
}


void PCB::setSS(unsigned int ss) {
	stackSegment = ss;
}

void PCB::setBP(unsigned int bp) {
	basePointer = bp;
}

void PCB::inic() {
	asm cli;

	oldInterrupt = getvect(8);
	setvect(8, timer);

	PCB::mainThread = new Thread(defaultStackSize, 2);
	mainThread->start(); // Places the main thread in the Scheduler
	running = Scheduler::get(); // The main thread is the only thread in the scheduler

	idle = new IdleThread();
	idle->start();

	asm sti;
}

void PCB::restore() {
	if (PCB::running != mainThread->myPCB) return;
	asm cli;
	setvect(8, oldInterrupt);
	// Delete all threads
	delete mainThread;
	delete idle;
	PCB::running = 0;
	//cout<<"Happy End"<<endl;
	asm sti;
}

void interrupt PCB::timer(...) {

	if (currentSlice < 0) currentSlice = PCB::running->getTimeSlice(); // counter was never initialized


	if(!PCB::reqContextSwitch) {
		currentSlice--;
		KernelSem::semList->markTick();
	}

	if (currentSlice == 0 || PCB::reqContextSwitch) {
		if (!PCB::locked) {
			PCB::reqContextSwitch = 0;
					asm {
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}

					PCB::running->setSP(tsp);
					PCB::running->setSS(tss);
					PCB::running->setBP(tbp);

					if (PCB::running->getState() == PCB::READY){
						Scheduler::put((PCB *) PCB::running);
					}

					while(1) {
						PCB::running = Scheduler::get();

						if (PCB::running == 0) PCB::running = idle->myPCB;

						if (PCB::running->getState() != PCB::READY) continue;

						tsp = PCB::running->getSP();
						tss = PCB::running->getSS();
						tbp = PCB::running->getBP();
						currentSlice = PCB::running->getTimeSlice();

						asm {
							mov sp, tsp
							mov ss, tss
							mov bp, tbp
						}
						break;
					}

					// SIGNALS //

					if (PCB::running != idle->myPCB) {
						// Run the signals that are requested and not blocked

						while(PCB::running->sigQueue->getSize() > 0) {
							SignalId sig = PCB::running->sigQueue->getElem();

							/*
							if(sig == 0) {
								// Kill the thread, and continue
								//PCB::running->sigArray[0]->runHandlers();
								PCB::running->signal0();
								break;
							}
							*/

							if(PCB::running->sigArray[sig] == 0) continue; // Signal not even registered / created

							if (!PCB::running->sigArray[sig]->isBlocked()) {
								//cout << "RUNNING " << sig << endl;
								PCB::running->sigArray[sig]->runHandlers();
							}

						}
					}

		} else {
			PCB::reqContextSwitch = 1;
		}
	}

	if(!PCB::reqContextSwitch) {
		tick(); // Tick is called when it was a real interrupt
		(*oldInterrupt)();
 	}
}

PCB::~PCB() {
	PCB::locked =1;
	delete sem;

	for(int i = 0; i < 16; i++) {
		if (sigArray[i] != 0) delete sigArray[i];
	}

	delete sigArray;
	delete sigQueue;
	PCB::locked = 0;
}

// SIGNALS //

// SigHead objects are created for signals that are actually being used

void PCB::initSigArray() {
	// Set the SigHead array to 0s
	sigArray = new SigHead*[16];

	for(int sig = 0; sig<16; sig++) {
		sigArray[sig] = 0;
	}
}

void PCB::signal(SignalId signal) {
	if (signal == 1 || signal == 2) {
		if(PCB::running->myThread != mainThread) return; // Only the system thread can call signals 1 and 2

		if (sigArray[signal] != 0 && !sigArray[signal]->isBlocked()) {
			/*
			PCB::locked = 1;
			cout << "SIG 1/2 by " << PCB::running->id << " for " << id << endl;
			asm cli;
			PCB::locked = 0;
			*/
			sigArray[signal]->runHandlers();
		} else {
			if (sigArray[signal] != 0) sigQueue->addElem(signal);
		}
	} else if (signal == 0) {
		signal0();
	} else {
		sigQueue->addElem(signal);
	}
}

void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	if(sigArray[signal] == 0) sigArray[signal] = new SigHead();
	sigArray[signal]->addHandler(handler);
}
void PCB::unregisterAllHandlers(SignalId id) {
	if(sigArray[id] == 0) return;
	sigArray[id]->removeHandlers();
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	if(sigArray[id] == 0) return;
	sigArray[id]->swapHandlers(hand1, hand2);
}

void PCB::blockSignal(SignalId signal) {
	if(sigArray[signal] == 0) sigArray[signal] = new SigHead();
	sigArray[signal]->changeState(1);
}
void PCB::blockSignalGlobally(SignalId signal) {
	threadList->blockSignal(signal);
}

void PCB::unblockSignal(SignalId signal) {
	if(sigArray[signal] == 0) sigArray[signal] = new SigHead();
	sigArray[signal]->changeState(0);
}
void PCB::unblockSignalGlobally(SignalId signal) {
	threadList->unblockSignal(signal);
}

void PCB::removeFromThreadList(ID id) {
	PCB::threadList->remove(PCB::running->id);
}

void PCB::freeSem() {
	while (PCB::running->sem->val() < 0) {
			PCB::running->sem->signal(0);
	}
}

void PCB::nullParent() {
	parent = 0;
}
//////////////////////////////////////////////////
