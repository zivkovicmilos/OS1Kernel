#ifndef IDLET_H_
#define IDLET_H_
#include "thread.h"
#include "pcb.h"

class IdleThread : public Thread {
public:
	IdleThread() : Thread() {}
	void start();
	void run();
};


#endif /* IDLET_H_ */
