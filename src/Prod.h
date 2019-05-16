#ifndef PROD_H_
#define PROD_H_
#include "thread.h"
#include "pcb.h"
#include "sem.h"
#include "buff.h"

class Producer : public Thread{
public:
	Producer(StackSize ss, Time ts, Buffer* buff) : Thread(ss, ts){this->buff = buff;}
	~Producer() {waitToComplete();}
protected:
	virtual void run();
private:
	Buffer* buff;
};




#endif /* PROD_H_ */
