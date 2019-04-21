#ifndef _pcbh_
#define _pcbh_

class PCB {
public:
	// PCB Flag declaration
	static enum threadState {NEW, READY, RUNNING, BLOCKED, FINISHED};
	// Global PCB running
	static PCB* running;
	static volatile bool reqContextSwitch;

	PCB(Thread* t, StackSize stackSize, Time timeSlice);

	threadState getState();
	void setState(threadState s);

	void initPCB();

	/*
	 * Statickoj metodi se adresa konstruise na steku u compile time, i njenu adresu mozemo dohvatiti
	 * Uvek cemo u Threadu da skacemo na adresu ovog wrappera kada podmecemo
	 *
	 */
	static void wrapper();

	unsigned int getId() const;

	void decTimeSlice();
	Time getTimeSlice();

	static void interrupt timer();

private:
	Thread* myThread;
	unsigned* stack;
	unsigned int id = ++cnt;
	static unsigned int cnt;
	StackSize ss;
	volatile Time ts;
	unsigned bp;

	volatile threadState state;

protected:

};

#endif
