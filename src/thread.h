#ifndef _thread_h_
#define _thread_h_
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;

class PCB; // Kernel's implementation of a user's thread

class Thread {
public:
	void start(); // ubacuje u Scheduler

	/*
	 * A kaze b.waitToComplete(), nit A se blokira u slucaju da nit B jos nije zavrsena (dok se ne zavrsi).
	 * Moramo pratiti za nit B ko ceka da se ona zavrsi, i kada se zavrsi da obavestimo sve koji cekaju i da ih odblokriamo.
	 * U svakoj niti imamo listu onih koji cekaju da se ona zavrsi, ali zapravo ovo uradimo sa jednim semaforom koji na pocetku
	 * stavimo da je 0. Svi oni koji hoce da se waituju na toj niti, oni cekaju na njenom semaforu.
	 * Kako znamo da se nit zavrsila?
	 * Tako sto je to deo u wrapperu posle njenog run-a.
	 * WaitToComplete nema efekta ako je nit zavrsena (imas getStatus u pcbu)
	 */
	void waitToComplete();

	/*
	 * Ovde proveravamo ako neko pokusava da obrise Thread, mora da saceka da se thread zavrsi
	 * treba da pozove waitToComplete
	 */
	virtual ~Thread();

	ID getId();

	static ID getRunningId();

	/*
	 * Treba da postoji struktura kojom pamtimo sve niti koje postoje u sistemu
	 */
	static Thread * getThreadById(ID id);

protected:
	friend class PCB;

	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);

	virtual void run() {}
private:
	PCB* myPCB;
};

/*
 * Treba da pozove interrupt timer
 */
void dispatch();

#endif
