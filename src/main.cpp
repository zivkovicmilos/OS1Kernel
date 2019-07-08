//#include "pcb.h"
//#include "prod.h"
//#include "cons.h"
#include "Event.h"
#include "ivte.h"
#include "user.h"

//PREPAREENTRY(252, 1);

//Event ev(252);
int userMain (int argc, char* argv[]);

int main(int argc, char* argv[]) {
	PCB::inic();

	int val = 0;
	// buffsize, broj producera, timeslice (100, 50, 5)
	val = userMain(argc, argv);

	PCB::restore();
	return val;
}
