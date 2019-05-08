#include "pcb.h"

unsigned int PCB::cnt = 0;
PCB* PCB::running = 0;
volatile int PCB::reqContextSwitch = 0;
bstTree* PCB::threads = new bstTree();

unsigned tss;
unsigned tsp;
unsigned tbp;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	// lock
	id = ++cnt;
	state = NEW;
	myThread = t;
	if (stackSize > maxStackSize) {
		this->stackSize = maxStackSize;
	} else {
		this->stackSize = stackSize;
	}
	this->timeSlice = timeSlice;
	stack = new unsigned[stackSize];
	threads->instBst(t);
	// unlock
}

void PCB::initPCB() {
	// lock

	tsp = FP_OFF(stack + stackSize-1);
	tss = FP_SEG(stack + stackSize-1);


	//unlock
}

Thread* PCB::findThread(ID id) {
	return PCB::threads->bstFind(id);
}

PCB::threadState PCB::getState() {
	return state;
}

void PCB::setState(threadState s) {
	state = s;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	running->setState(FINISHED);
	// OVDE IDU SVE STVARI KOJE ZELIM DA SE DESE KADA SE NIT ZAVRSI
	// signalAll, zbog waitToComplete
	// dispatch();
}

void PCB::decTimeSlice() {
	// lock
	running->timeSlice--;
	// unlock
}

void interrupt PCB::timer() {
	if(!PCB::reqContextSwitch) {
		PCB:running->decTimeSlice();
		// asm int 60h; ?
	}

	if (PCB::running->getTimeSlice() == 0 || PCB::reqContextSwitch) {
		// obrati paznju na ovaj prvi asm block
		asm {
			mov tsp, stack
			mov tss, ss
			mov tbp, bp
		}
		running->stack = tsp;
		running->stackSize = tss;
		running->bp = tbp;

		running = Scheduler::get();

		tsp = running->stack;
		tss = running->stackSize;
		tbp = running->bp;

		asm {
			mov sp, tsp
			mov ss, tss
			mov bp, tbp
		}
	}

	if(!PCB::reqContextSwitch) {
		asm int 60h;
	}

	reqContextSwitch = 0;
}
