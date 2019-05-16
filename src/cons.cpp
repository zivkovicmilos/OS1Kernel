#include "cons.h"

int Consumer::cnt = 0;

void Consumer::run() {
	int item = 0;
	while (1) {
		item = buff->getItem();
		PCB::locked = 1;
		cout << "Consumer #"<< id << " took: " << item << endl;
		asm cli;
		PCB::locked = 0;
		if(PCB::reqContextSwitch) dispatch();
		for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
	}
}
