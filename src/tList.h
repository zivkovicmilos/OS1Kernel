#ifndef TLIST_H_
#define TLIST_H_
#include "thread.h"

class ThreadList {
	struct Elem {
		Thread* data;
		Elem* next;
		Elem(Thread* d) {
			data = d;
			next = 0;
		}
	};
	Elem* first;
	Elem* last;
	int num;
public:
	friend class PCB;
	ThreadList();
	void add(Thread*);
	void remove(ID);
	// SIGNALS //
	void blockSignal(SignalId signal);
	void unblockSignal(SignalId signal);
	////////////////////////////////////
	Thread* getByID(ID);
	~ThreadList();
};



#endif /* TLIST_H_ */
