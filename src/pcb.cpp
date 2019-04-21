#include "pcb.h"

int PCB::cnt = 0;
PCB* PCB::running = nullptr;
bool PCB::reqContextSwitch = false;

unsigned tss;
unsigned tsp;
unsigned tbp;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	// lock
	state = NEW;
	myThread = t;
	ss = stackSize;
	ts = timeSlice;
	stack = new unsigned[stackSize];
	// unlock
}

void PCB::initPCB() {
	// Imas u onom Plakalovic dokumentu
}

unsigned int PCB::getId() const {
	return id;
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
	running->ts--;
	// unlock
}

void interrupt PCB::timer() {
	if(!PCB::reqContextSwitch) {
		decTimeSlice();
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
		running->ss = tss;
		running->bp = tbp;

		running = Scheduler::get();

		tsp = running->stack;
		tss = running->ss;
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

	reqContextSwitch = false;
}
