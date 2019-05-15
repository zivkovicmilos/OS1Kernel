#include "idleT.h"

void IdleThread::run() {
	int num = 1;
	while (num>0) {
		num+=2;
		num = 1; // useless operation
	};
}

void IdleThread::start() {
	if (myPCB->getState() == PCB::NEW) {
			myPCB->initPCB();
			myPCB->setState(PCB::READY);
	}
}
