//#include "pcb.h"
#include "A.h"
#include "B.h"

int userMain(int argc, char* argv[]) {
		PCB::locked = 1;
		A* a = new A(4096, 5);
		B* b = new B(4096, 3);
		a->start();
		b->start();
		//cout<<"napravio a"<<endl;
		//asm cli;
		PCB::locked = 0;
		delete a;
		delete b;

		/*
		B* b = new B(4096, 20);
		b->start();
		cout<<"napravio b"<<endl;
		asm cli;
		PCB::locked = 0;

		delete a;
		PCB::locked = 1;
		cout<<"ZAVRSIO SA A"<<endl;
		asm cli;
		PCB::locked = 0;
		delete b;
		*/

	return 0;
}

int main(int argc, char* argv[]) {
	PCB::inic();

	int val = 0;
	val = userMain(argc, argv);

	PCB::restore();
	return val;
}
