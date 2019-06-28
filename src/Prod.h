#ifndef PROD_H_
#define PROD_H_
#include "thread.h"
#include "pcb.h"
#include "buff.h"
#include "sem.h"

class Prodducer : public Thread{
public:
	Prodducer(StackSize ss, Time ts, Buffer* buff) : Thread(ss, ts){this->buff = buff;}
	~Prodducer() {waitToComplete();}
protected:
	virtual void ruan();
private:
	Buffer* buff;
};




#endif /* PROD_H_ */
