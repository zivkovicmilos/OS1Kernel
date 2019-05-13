#include "B.h"

void B::run () {
			for (int i =0; i < 30; ++i) {
					PCB::locked = 1;
					cout<<"B i = "<<i<<endl;
					asm cli;
					PCB::locked = 0;
					if(PCB::reqContextSwitch) dispatch();

					for (int k = 0; k<10000; ++k)
						for (int j = 0; j <30000; ++j);
			}
			PCB::locked = 1;
			cout<<"Exiting B..." << endl;
			asm cli;
			PCB::locked = 0;
}
