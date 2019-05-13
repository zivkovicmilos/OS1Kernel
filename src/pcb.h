#ifndef _pcbh_
#define _pcbh_
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
#include "thread.h"
#include <stdlib.h>
#include <stdio.h>
#include "bstT.h"
#include "queue.h"
const unsigned long maxStackSize = 16384; // TODO check

static void interrupt(*oldInterrupt)(...);

class PCB {
public:
	// PCB Flag declaration
	static enum threadState {NEW, READY, RUNNING, BLOCKED, FINISHED};
	// Global PCB running
	static PCB* running;
	static void interrupt timer(...);
	static volatile unsigned int locked;
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

	Time getTimeSlice();
	//static void interrupt timer();
	static Thread* findThread(ID);
	static void inic();
	static void restore();
	ID id;
	static volatile unsigned int activeThreads;
	friend class MainThread;

private:
	friend class Thread;

	Thread* myThread;
	static Thread* mainThread;
	Queue* waiting;
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
