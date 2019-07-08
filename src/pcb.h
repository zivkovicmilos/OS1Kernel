#ifndef _pcbh_
#define _pcbh_
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>
#include "thread.h"
#include <stdlib.h>
#include <stdio.h>

#include "sem.h"
#include "tList.h"

#include "sighead.h"
#include "sigq.h"

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

	PCB(Thread*, StackSize, Time);
	~PCB();

	threadState getState();
	void setState(threadState s);
	void setSP(unsigned int);
	void setSS(unsigned int);
	void setBP(unsigned int);

	unsigned int getSP() const;
	unsigned int getSS() const;
	unsigned int getBP() const;

	void initPCB();

	static void wrapper();
	static void sigWrapper();

	Time getTimeSlice();
	static Thread* findThread(ID);
	static void inic();
	static void restore();
	ID id;

	// SIGNALS //
	void initSigArray();
	void signal (SignalId signal);
	void signal0();

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	static void removeFromThreadList(ID id);
	void freeSem();
	void nullParent();

	/////////////////////////////////////////////////////
	Thread* parent;
private:
	friend class Thread;
	friend class KernelSem;
	friend class KernelEv;

	static ThreadList* threadList;
	Thread* myThread;

	static Thread* mainThread;
	static IdleThread* idle;
	Semaphore* sem;
	static unsigned int cnt;
	StackSize stackSize;
	volatile Time timeSlice;
	unsigned stackPointer;
	unsigned stackSegment;
	unsigned basePointer;
	volatile threadState state;

	// SIGNALS //
	SigHead** sigArray;
	SigQueue* sigQueue;
};

#endif
