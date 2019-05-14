#ifndef _waitLH_
#define _waitLH_

#include "pcb.h"

class WaitList {
	struct Elem {
		Elem* next;
		Elem* prev;
		int val;
		Thread* t;
		Elem(Thread* t, int val) {
			this->val = val;
			this->t = t;
			next = 0;
			prev = 0;
		}
	};

	Elem* first;
	int numOfWaiting;

public:
	WaitList();
	void addWaiting(Thread*, int);
	void removeWaiting();

	int getNumOfWaiting() const; // Maybe useless, because of friend class

	void printList();

	~WaitList();
};

#endif // !_waitL.H_
