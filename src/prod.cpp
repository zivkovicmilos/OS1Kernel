#include "prod.h"

void Prodducer::ruan() {
	int num = 0;
	while(1) {
			buff->addItem(num);
			PCB::locked = 1;
			cout << "Added: " << num << endl;
			num = (num+1)%100;
			asm cli;
			PCB::locked = 0;
			if(PCB::reqContextSwitch) dispatch();

			for (int k = 0; k<10000; ++k)
					for (int j = 0; j <30000; ++j);
	}
}
