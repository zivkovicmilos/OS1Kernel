#ifndef CONS_H_
#define CONS_H_
#include "thread.h"
#include "pcb.h"
#include "buff.h"
#include "sem.h"

class Conssumer : public Thread{
public:
	Conssumer(StackSize ss, Time ts, Buffer* buff) : Thread(ss, ts){this->buff = buff; id =++cnt;}
	~Conssumer() {waitToComplete();}
protected:
	virtual void ruan();
private:
	Buffer* buff;
	int id;
	static int cnt;
};


#endif /* CONS_H_ */
