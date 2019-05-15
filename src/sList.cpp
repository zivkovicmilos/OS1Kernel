#include "sList.h"
#include "ksem.h"

SemList::SemList() {
	first = last = 0;
	num = 0;
}

void SemList::add(Semaphore* t) {
	if (first == 0) {
		first = new Elem(t);
		last = first;
	} else {
		Elem* temp = new Elem(t);
		last->next = temp;
		last = temp;
	}
	num++;
}

SemList::~SemList() {
	Elem* temp = first;
	Elem* prev = 0;
	while (temp != 0) {
		prev = temp;
		temp = temp->next;
		delete prev;
		// TODO maybe more?
	}
	first = last = 0;
	num = 0;
}

void SemList::markTick() {
	Elem* temp = first;
	while (temp != 0) {
		temp->data->myImpl->limitedWait->removeWaiting();
		temp = temp->next;
	}
}
