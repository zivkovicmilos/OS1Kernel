#ifndef B_H_
#define B_H_
#include "pcb.h"

class B : public Thread {
public:
	B(StackSize ss, Time ts) : Thread(ss, ts){}
	~B() {waitToComplete();}
	void run();
};


#endif /* B_H_ */
