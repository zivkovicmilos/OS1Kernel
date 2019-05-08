#ifndef _pcbh_
#define _pcbh_
#include "SCHEDULE.H"
#include "thread.h"
#include <dos.h>
#include <stdlib.h>
#include "bstT.h"
const unsigned long maxStackSize = 16384; // TODO check

class PCB {
public:
	// PCB Flag declaration
	static enum threadState {NEW, READY, RUNNING, BLOCKED, FINISHED};
	// Global PCB running
	static PCB* running;
	static volatile int reqContextSwitch;
	static bstTree* threads;

	PCB(Thread*, StackSize, Time);

	threadState getState();
	void setState(threadState s);

	void initPCB();

	/*
	 * Statickoj metodi se adresa konstruise na steku u compile time, i njenu adresu mozemo dohvatiti
	 * Uvek cemo u Threadu da skacemo na adresu ovog wrappera kada podmecemo
	 *
	 */
	static void wrapper();

	void decTimeSlice();
	Time getTimeSlice();

	static void interrupt timer();
	static Thread* findThread(ID);

private:
	Thread* myThread;
	unsigned* stack;
	ID id;
	static unsigned int cnt;
	StackSize stackSize;
	volatile Time timeSlice;
	unsigned bp; // Base pointer
	volatile threadState state;


protected:

};

#endif
