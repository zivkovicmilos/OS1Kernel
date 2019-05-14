#ifndef KSEM_H_
#define KSEM_H_

#include "waitL.h"
#include "queue.h"
//#include "sem.h"
typedef unsigned int Time;

class KernelSem {
public:
	KernelSem(int init = 1);

	int wait(Time maxTimeToWait);
	int signal(int n=0);

	int val () const;
	~KernelSem();
private:
	friend class Semaphore;

	int value;
	WaitList* limitedWait;
	Queue* unlimitedWait;
};



#endif /* KSEM_H_ */
