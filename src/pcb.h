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
	static volatile unsigned int lockFlag;
	static volatile int reqContextSwitch;
	static bstTree* threads;

	PCB(Thread*, StackSize, Time);

	threadState getState();
	void setState(threadState s);
	void setSP(unsigned int);
	void setSS(unsigned int);
	void setBP(unsigned int);

	unsigned int getSP() const;
	unsigned int getSS() const;
	unsigned int getBP() const;

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
	static void inic();
	static void restore();
	static unsigned oldTimerOFF, oldTimerSEG;
	ID id;
private:
	Thread* myThread;
	static unsigned int cnt;
	StackSize stackSize;
	volatile Time timeSlice;
	unsigned stackPointer;
	unsigned stackSegment;
	unsigned basePointer;
	volatile threadState state;


protected:

};

#endif
