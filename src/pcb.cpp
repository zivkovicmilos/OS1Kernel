#include "pcb.h"

unsigned int PCB::cnt = 0;
unsigned int PCB::lockFlag = 1;
PCB* PCB::running = 0;
volatile int PCB::reqContextSwitch = 0;
bstTree* PCB::threads = new bstTree();

unsigned tss;
unsigned tsp;
unsigned tbp;

PCB::PCB(Thread* t, StackSize stackSize, Time timeSlice) {
	// lock
	lockFlag = 0;
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
	threads->instBst(t);
	lockFlag = 1;
	// unlock
}

void PCB::initPCB() {
	// lock
	lockFlag = 0;
	unsigned* stack = new unsigned[stackSize];

	stack[stackSize-1] = 0x200; // Set the I flag

	stack[stackSize-2] = FP_SEG(PCB::wrapper());
	stack[stackSize-3] = FP_OFF(PCB::wrapper());

	stackPointer = FP_OFF(stack+stackSize-12); // 12 bytes of regs are saved when entering the interrupt
	stackSegment = FP_SEG(stack+stackSize-12);
	basePointer = FP_OFF(stack+stackSize-12);
	lockFlag = 1;
	//unlock
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

void PCB::wrapper() {
	PCB::running->myThread->run();
	running->setState(FINISHED);
	// OVDE IDU SVE STVARI KOJE ZELIM DA SE DESE KADA SE NIT ZAVRSI
	// signalAll, zbog waitToComplete
	// dispatch();
}

void PCB::decTimeSlice() {
	// lock
	lockFlag = 0;
	running->timeSlice--;
	lockFlag = 1;
	// unlock
}

void interrupt PCB::timer() {
	if(!PCB::reqContextSwitch) {
		PCB:running->decTimeSlice();
		// asm int 60h; ?
	}

	if (PCB::running->getTimeSlice() == 0 || PCB::reqContextSwitch) {
		if (lockFlag) {
			PCB::reqContextSwitch = 0;
			// obrati paznju na ovaj prvi asm block
					unsigned ttsp, ttss, ttbp;
					ttsp = running->getSP();
					ttss = running->getSS();
					ttbp = running->getBP();
					// TODO ovo ne valja
					asm {
						mov tsp, ttsp
						mov tss, ttss
						mov tbp, ttbp
					}

					lockFlag = 0;
					running->setSP(tsp);
					running->setSS(tss);
					running->setBP(tbp);
					lockFlag = 1;

					if (!(running->getState() != FINISHED)) Scheduler::put((PCB *) running);
					running = Scheduler::get();

					lockFlag = 0;
					tsp = running->getSP();
					tss = running->getSS();
					tbp = running->getBP();
					lockFlag = 1;

					asm {
						mov ttsp, tsp
						mov ttss, tss
						mov ttbp, tbp
					}
					running-
		}
	}
	if(!PCB::reqContextSwitch) {
		asm int 60h;
	}
	reqContextSwitch = 0;
}
unsigned PCB::oldTimerOFF = 0
unsigned PCB::oldTimerSEG = 0;

void PCB::inic() {
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}
}

void PCB::restore() {
	asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
		}
}
