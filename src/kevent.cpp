#include "kevent.h"

KernelEv::KernelEv(IVTNo num) {
	owner = PCB::running->myThread;
	entry = num;
	waitList = new Queue();
	IVTEntry::entryArray[entry]->setMyEvent(this);
}


void KernelEv::wait() {
	if (PCB::running->myThread != owner) return;
	PCB::locked = 1;
	PCB::running->setState(PCB::BLOCKED);
	waitList->addElem(PCB::running);
	PCB::locked = 0;
	dispatch();
}

void KernelEv::signal() {
	PCB::locked = 1;
	PCB* temp = 0;
	while (waitList->getSize() > 0) {
		temp = waitList->getElem();
		temp->setState(PCB::READY);
		Scheduler::put(temp);
	}
	PCB::locked = 0;
	// dispatch?
}

KernelEv::~KernelEv() {
	delete waitList;
	delete IVTEntry::entryArray[entry];
	owner = 0;
	entry = 0;
}
