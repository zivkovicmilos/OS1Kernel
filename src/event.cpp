#include "Event.h"
#include "kevent.h"
#include "pcb.h"

Event::Event(IVTNo ivtNo) {
	PCB::locked = 1;
	myImpl = new KernelEv(ivtNo);
	PCB::locked = 0;
}

Event::~Event() {
	PCB::locked = 1;
	delete myImpl;
	PCB::locked = 0;
}

void Event::wait() {
	myImpl->wait();
}

void Event::signal() {
	myImpl->signal();
}
