#include "pcb.h"

int PCB::cnt = 0;
PCB* PCB::running = nullptr;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	myThread = t;
	ss = stackSize;
	ts = timeSlice;
}

unsigned int PCB::getId() const {
	return id;
}

PCB::threadState PCB::getState() {
	return state;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	// OVDE IDU SVE STVARI KOJE ZELIM DA SE DESE KADA SE NIT ZAVRSI
	// signalAll, zbog waitToComplete
}
