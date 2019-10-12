#include "Event.h"
#include "ivte.h"
#include "user.h"
int userMain (int argc, char* argv[]);

int main(int argc, char* argv[]) {
	PCB::inic();

	int val = 0;
	// buffsize, broj producera, timeslice (100, 50, 5)
	val = userMain(argc, argv);

	PCB::restore();
	return val;
}
