#ifndef SLIST_H_
#define SLIST_H_
#include "sem.h"

class SemList {
	struct Elem {
		Semaphore* data;
		Elem* next;
		Elem(Semaphore* d) {
			data = d;
			next = 0;
		}
	};
	Elem* first;
	Elem* last;
	int num;
public:
	friend class Semaphore;
	SemList();
	void markTick();
	void add(Semaphore*);
	~SemList();
};



#endif /* SLIST_H_ */
