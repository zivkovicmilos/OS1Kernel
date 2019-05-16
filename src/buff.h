#ifndef BUFF_H_
#define BUFF_H_
#include "sem.h"
#include "pcb.h"

const int N = 100;

class Buffer {
	unsigned int arr[N];
	//Semaphore* mutex;
	Semaphore* spaceAvailable;
	Semaphore* itemAvailable;
	int head, tail;

public:
	Buffer() {
		//mutex = new Semaphore(1);
		spaceAvailable = new Semaphore(100);
		itemAvailable = new Semaphore(0);
		head = tail = 0;
	}
	void addItem(unsigned int num) {
		spaceAvailable->wait(0);
		//mutex->wait(0);
		PCB::locked = 1;
		arr[tail] = num;
		tail = (tail+1)%N;
		//mutex->signal(0);
		PCB::locked = 0;
		itemAvailable->signal(0);
	}

	int getItem() {
		itemAvailable->wait(0);
		//mutex->wait(0);
		PCB::locked = 1;
		int ret = arr[head];
		head = (head+1)%N;
		//mutex->signal(0);
		PCB::locked = 0;
		spaceAvailable->signal(0);
		return ret;
	}
};



#endif /* BUFF_H_ */
