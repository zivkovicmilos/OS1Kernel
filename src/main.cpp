//#include "pcb.h"
//#include "prod.h"
//#include "cons.h"
#include "Event.h"
#include "ivte.h"
#include "user.h"

//PREPAREENTRY(252, 1);

//Event ev(252);
int userMain (int argc, char* argv[]);

int myMain(int argc, char* argv[]) {
		//PCB::locked = 1;
		//Buffer* b = new Buffer();
		//Producer* p = new Producer(4096, 5, b);
		//Consumer* consOne = new Consumer(4096, 3, b);
		//Consumer* consTwo = new Consumer(4096, 3, b);
		//p->start();
		//consOne->start();
		//consTwo->start();
		//PCB::locked = 0;
		//delete p;
		//delete consOne;
		//delete consTwo;

	return 0;
}

int main(int argc, char* argv[]) {
	PCB::inic();

	int val = 0;
	// buffsize, broj producera, timeslice (100, 50, 5)
	val = userMain(argc, argv);

	PCB::restore();
	return val;
}
