#ifndef PROD_H_
#define PROD_H_
#include "thread.h"

class Producer {
	Producer(int numOfItems) : n(numOfItems) {}
	virtual ~Producer() {
		waitToComplete();
	}
protected:
	virtual void run() {
		for (int i =0; i<n; i++) {
			PCB::locked = 1;
			cout<<"Prod "<<id<< ": "<< i<<endl;
			asm cli;
			PCB::locked = 0;
			if(PCB::reqContextSwitch) dispatch();

			for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
		}
	}
private:
	int n;
	int id;
};




#endif /* PROD_H_ */
