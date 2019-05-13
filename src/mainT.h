#ifndef MAINT_H_
#define MAINT_H_
#include "thread.h"
//extern unsigned int activeThreads;

class MainThread : public Thread {
public:
	MainThread(StackSize ss, Time ts) : Thread(ss, ts){}
	void run() {}

};




#endif /* MAINT_H_ */
