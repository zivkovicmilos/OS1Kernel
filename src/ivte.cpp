#include "ivte.h"

IVTEntry** IVTEntry::entryArray = new IVTEntry*[256];

IVTEntry::IVTEntry(IVTNo entryNum, IntRoutine newRoutine) {

	oldRoutine = getvect(entryNum);

	setvect(entryNum, newRoutine);

	IVTEntry::entryArray[entryNum] = this;
}

void IVTEntry::signal() {
	myEvent->signal();
}

void IVTEntry::old() {
	(*oldRoutine)();
}


void IVTEntry::setMyEvent(KernelEv* event) {
	myEvent = event;
}

IVTEntry::~IVTEntry() {
	setvect(myEvent->entry, oldRoutine);
	IVTEntry::entryArray[myEvent->entry] = 0;

}
