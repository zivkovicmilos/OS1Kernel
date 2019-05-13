#include "idleT.h"

void IdleThread::run() {
	while (1) {};
}

void IdleThread::start() {
	if (myPCB->getState() == PCB::NEW) {
			myPCB->initPCB();
			myPCB->setState(PCB::READY);
	}
}
