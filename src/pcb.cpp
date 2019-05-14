#include "pcb.h"
#include "idleT.h"
class MainThread;
unsigned int PCB::cnt = 0;
volatile unsigned int PCB::activeThreads = 0;
static IdleThread* idle = 0;
Thread* PCB::mainThread = 0;
volatile unsigned int PCB::locked = 0;
PCB* PCB::running = 0;
volatile int PCB::reqContextSwitch = 0;
bstTree* PCB::threads = new bstTree();

unsigned tss;
unsigned tsp;
unsigned tbp;

volatile int currentSlice = -1;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	// lock
	PCB::locked = 1;
	//PCB::activeThreads++;
	id = ++cnt;
	state = NEW;
	myThread = t;
	if (stackSize > maxStackSize) {
		this->stackSize = maxStackSize;
	} else {
		this->stackSize = stackSize;
	}
	this->timeSlice = timeSlice;
	stackPointer = stackSegment = basePointer = 0;
	//threads->instBst(t);
	sem = new Semaphore(0);
	PCB::locked = 0;
	// unlock
}

Time PCB::getTimeSlice() {
	return timeSlice;
}

void PCB::wrapper() {
	PCB::running->myThread->run();
	running->setState(FINISHED);
	//PCB::activeThreads--;
	// OVDE IDU SVE STVARI KOJE ZELIM DA SE DESE KADA SE NIT ZAVRSI
	// signalAll, zbog waitToComplete
	while (PCB::running->sem->val() < 0) {
		PCB::running->sem->signal(0);
	}
	dispatch();
	/*
	 * CHANGED
	PCB* temp;
	while (PCB::running->waiting->getSize() > 0) {
		temp = running->waiting->getElem();
		temp->setState(PCB::READY);
		PCB::locked = 1;
		cout << "Stavljam nit: " << temp->id << endl;
		PCB::locked = 0;
		Scheduler::put(temp);
	}
	dispatch();
	*/
}

void (*body)() = PCB::wrapper;

void PCB::initPCB() {
	unsigned* stack = new unsigned[stackSize];
	stack[stackSize-1] = 0x200; // Set the I flag

	stack[stackSize-2] = FP_SEG(body);
	stack[stackSize-3] = FP_OFF(body);

	stackPointer = FP_OFF(stack+stackSize-12); // 12 bytes of regs are saved when entering the interrupt
	stackSegment = FP_SEG(stack+stackSize-12);
	basePointer = FP_OFF(stack+stackSize-12);
}

Thread* PCB::findThread(ID id) {
	return PCB::threads->bstFind(id);
}

PCB::threadState PCB::getState() {
	return state;
}

void PCB::setState(threadState s) {
	state = s;
}

void PCB::setSP(unsigned int sp) {
	stackPointer = sp;
}

unsigned int PCB::getSP() const {
	return stackPointer;
}

unsigned int PCB::getSS() const {
	return stackSegment;
}

unsigned int PCB::getBP() const {
	return basePointer;
}


void PCB::setSS(unsigned int ss) {
	stackSegment = ss;
}

void PCB::setBP(unsigned int bp) {
	basePointer = bp;
}

void PCB::inic() {
	asm cli;

	oldInterrupt = getvect(8);
	setvect(8, timer);

	PCB::mainThread = new Thread(4096, 2);
	mainThread->start(); // Places the main thread in the Scheduler
	running = Scheduler::get(); // The main thread is the only thread in the scheduler

	asm sti;
}

void PCB::restore() {
	if (PCB::running != mainThread->myPCB) return;
	asm cli;
	setvect(8, oldInterrupt);
	// Delete all threads
	delete mainThread;
	delete mainThread->myPCB;
	PCB::running = 0;
	cout<<"Happy End"<<endl;
	asm sti;
}

void interrupt PCB::timer(...) {

	if (currentSlice < 0) currentSlice = PCB::running->getTimeSlice(); // counter was never initialized

	if(!PCB::reqContextSwitch) {
		currentSlice--;
	}

	if (currentSlice == 0 || PCB::reqContextSwitch) {
		if (!PCB::locked) {
			PCB::reqContextSwitch = 0;
			// obrati paznju na ovaj prvi asm block
					asm {
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}

					PCB::running->setSP(tsp);
					PCB::running->setSS(tss);
					PCB::running->setBP(tbp);

					PCB::locked = 1;
					cout << "Menja se kontekst sa " << PCB::running->id << " NUM: "<< PCB::activeThreads << endl;
					asm cli;
					PCB::locked = 0;

					if (PCB::running->getState() == PCB::READY){
						Scheduler::put((PCB *) PCB::running);
					}

					PCB* temp = 0;
					while(1) {
						temp = PCB::running = Scheduler::get();
						PCB::locked = 1;
						cout << "Izabrana nit: " << PCB::running->id << endl;
						asm cli;
						PCB::locked = 0;

						//if (temp == 0) PCB::running = idle->myPCB; CHANGED UNCOMMENT!!!!

						if (temp->getState() != PCB::READY) continue;

						tsp = PCB::running->getSP();
						tss = PCB::running->getSS();
						tbp = PCB::running->getBP();
						currentSlice = PCB::running->getTimeSlice();

						asm {
							mov sp, tsp
							mov ss, tss
							mov bp, tbp
						}
						break;
					}

		} else {
			PCB::reqContextSwitch = 1;
		}
	}

	if(!PCB::reqContextSwitch) {
		//void tick(); Tick zovemo samo kad je stvarno doslo do prekida
		oldInterrupt();
	}
}
