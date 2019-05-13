#include "queue.h"

Queue::Queue() {
	first = 0;
	last = 0;
	size = 0;
}

void Queue::addElem(PCB* pcb) {
	if (first == 0) {
		first = new Elem(pcb);
		last = first;
	} else {
		Elem* temp = new Elem(pcb);
		last->next = temp;
		last = temp;
	}
	size++;
}

PCB* Queue::getElem() {
	if (first == 0) return 0;

	PCB* ret = 0;
	if (first->next != 0) {
		ret = first->data;
		Elem* temp = first;
		first = first->next;
		delete temp;
	} else {
		ret = first->data;
		delete first;
		first = last = 0;
	}
	size--;
	return ret;
}

int Queue::getSize() {
	return size;
}

Queue::~Queue() {
	Elem* temp = first;
	Elem* prev = 0;
	while (temp != 0) {
		prev = temp;
		temp = temp->next;
		delete prev;
	}
	first = last = 0;
	size = 0;
}
