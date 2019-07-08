#include "pcb.h"
#include "idleT.h"
#include "ksem.h"

unsigned int PCB::cnt = 0;
//volatile unsigned int PCB::activeThreads = 0;
static IdleThread* idle = 0;
Thread* PCB::mainThread = 0;
volatile unsigned int PCB::locked = 0;
ThreadList* PCB::threadList = new ThreadList();
PCB* PCB::running = 0;
volatile int PCB::reqContextSwitch = 0;
//int PCB::sigWiped = 0;
//SigHead** PCB::sigArray = new SigHead*[16];

unsigned tss;
unsigned tsp;
unsigned tbp;

void tick();

volatile int currentSlice = -1;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	// lock
	PCB::locked = 1;
	//PCB::activeThreads++;

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
	//threads->instBst(t);
	sem = new Semaphore(0);
	PCB::locked = 0;
	// unlock
}

Time PCB::getTimeSlice() {
	return timeSlice;
}

void PCB::wrapper() {
	PCB::running->myThread->run();

	if(PCB::running->getState() != PCB::FINISHED) { // TODO changed 28.6
		running->setState(FINISHED);
			threadList->remove(PCB::running->id);

			// SIGNALS // TODO Changed 28.6 19:00
			if(PCB::running->parent != 0) PCB::running->parent->signal(1);
			PCB::running->signal(2); // To itself

			//PCB::activeThreads--;
			// OVDE IDU SVE STVARI KOJE ZELIM DA SE DESE KADA SE NIT ZAVRSI
			// signalAll, zbog waitToComplete

			// Do all this even if
			while (PCB::running->sem->val() < 0) {
				PCB::running->sem->signal(0);
			}
			dispatch();
			/*
			 * CHANGED
			PCB* temp;
			while (PCB::running->waiting->getSize() > 0) {
				temp = running->waiting->getElem();
				temp->setState(PCB::READY);
				PCB::locked = 1;
				cout << "Stavljam nit: " << temp->id << endl;
				PCB::locked = 0;
				Scheduler::put(temp);
			}
			dispatch();
			*/
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
	//delete mainThread->myPCB;
	PCB::running = 0;
	cout<<"Happy End"<<endl;
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
			// obrati paznju na ovaj prvi asm block
					asm {
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}

					PCB::running->setSP(tsp);
					PCB::running->setSS(tss);
					PCB::running->setBP(tbp);

					/*
					PCB::locked = 1;
					cout << "Menja se kontekst sa " << PCB::running->id << endl;
					asm cli;
					PCB::locked = 0;
					*/


					if (PCB::running->getState() == PCB::READY){
						Scheduler::put((PCB *) PCB::running);
					}

					while(1) {
						PCB::running = Scheduler::get();

						/*
						PCB::locked = 1;
						cout << "Izabrana nit: " << PCB::running->id << endl;
						asm cli;
						PCB::locked = 0;
						*/

						if (PCB::running == 0) PCB::running = idle->myPCB;

						if (PCB::running->getState() != PCB::READY) continue; // TODO Check

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

							if(sig == 0) {
								// Kill the thread, and continue
								PCB::running->sigArray[0]->runHandlers();
								break;
							}

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
		tick(); //Tick zovemo samo kad je stvarno doslo do prekida
		(*oldInterrupt)(); // CHANGED
 	}
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

// TODO add that only the system thread can call signal 1 / 2
void PCB::signal(SignalId signal) {
	if (signal == 2) {
		if (PCB::running->sigArray[signal] != 0 && !PCB::running->sigArray[signal]->isBlocked()) {
			//cout << "RUNNING " << sig << endl;
			PCB::running->sigArray[signal]->runHandlers();
		}
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
