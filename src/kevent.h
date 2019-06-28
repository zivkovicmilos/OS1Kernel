#ifndef KEVENT_H_
#define KEVENT_H_
#include "Event.h"
#include "pcb.h"
#include "ivte.h"
#include "queue.h"

class KernelEv {
	Thread* owner; // In constructor, thread running
	IVTNo entry;
	Queue* waitList;
	friend class IVTEntry;
public:
	KernelEv(IVTNo);
	~KernelEv();
	void signal();
	void wait();
};



#endif /* KEVENT_H_ */
