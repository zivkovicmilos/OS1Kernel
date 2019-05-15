#ifndef B_H_
#define B_H_
#include "pcb.h"
#include "sem.h"

class B : public Thread {
public:
	B(StackSize ss, Time ts) : Thread(ss, ts){ sem = new Semaphore();}
	~B() {waitToComplete();}
	void run();
private:
	Semaphore* sem;
};


#endif /* B_H_ */
