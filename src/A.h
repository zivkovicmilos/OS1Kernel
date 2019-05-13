#ifndef A_H_
#define A_H_
#include "pcb.h"

class A : public Thread {
public:
	A(StackSize ss, Time ts) : Thread(ss, ts){}
	~A() {waitToComplete();}
	void run();
};


#endif /* A_H_ */
