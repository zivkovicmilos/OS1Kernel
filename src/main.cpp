#include "thread.h"
#include "pcb.h"
#include <iostream.h>
#include <dos.h>

void mainFunct() {

}

int main() {
	Thread* idle = new Thread();
	PCB::inic();

	mainFunct();

	restore();
}
