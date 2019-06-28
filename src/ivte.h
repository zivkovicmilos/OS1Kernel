#ifndef IVTE_H_
#define IVTE_H_
#include "dos.h"
#include "kevent.h"

typedef void interrupt (*IntRoutine)(...);

#define PREPAREENTRY(num, callOld) \
		void interrupt intr##num(...); \
		IVTEntry ivte##num(num, intr##num); \
		void interrupt intr##num(...) { \
			if (callOld) ivte##num.old(); \
			ivte##num.signal(); \
		};

//#define GETADR(n) getvect(intr##n)

class IVTEntry {
	KernelEv* myEvent;
	IntRoutine oldRoutine;
public:
	IVTEntry(IVTNo, IntRoutine); // ovde cuva staru prekidnu rutinu
	~IVTEntry();
	void signal();
	void old();

	void setMyEvent(KernelEv*);
	static IVTEntry** entryArray;
};



#endif /* IVTE_H_ */
