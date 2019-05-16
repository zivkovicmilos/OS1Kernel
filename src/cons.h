#ifndef CONS_H_
#define CONS_H_
#include "thread.h"
#include "pcb.h"
#include "sem.h"
#include "buff.h"

class Consumer : public Thread{
public:
	Consumer(StackSize ss, Time ts, Buffer* buff) : Thread(ss, ts){this->buff = buff; id =++cnt;}
	~Consumer() {waitToComplete();}
protected:
	virtual void run();
private:
	Buffer* buff;
	int id;
	static int cnt;
};


#endif /* CONS_H_ */
