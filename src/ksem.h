#ifndef KSEM_H_
#define KSEM_H_

#include "waitL.h"
#include "queue.h"
#include "sList.h"
typedef unsigned int Time;

class KernelSem {
public:
	KernelSem(Semaphore*, int init = 1);

	int wait(Time maxTimeToWait);
	int signal(int n=0);

	int val () const;
	~KernelSem();
private:
	friend class Semaphore;
	friend class PCB;
	friend class SemList;
	static SemList* semList;
	int value;
	WaitList* limitedWait;
	Queue* unlimitedWait;
	Semaphore* mySem;
};



#endif /* KSEM_H_ */
