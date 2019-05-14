#ifndef A_H_
#define A_H_
#include "pcb.h"

class A : public Thread {
public:
	A(StackSize ss, Time ts) : Thread(ss, ts){idBroj =tren++;}
	~A() {waitToComplete();}
	void run();
private:
	int idBroj;
	static int tren;
};


#endif /* A_H_ */
