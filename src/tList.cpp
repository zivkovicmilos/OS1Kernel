#include "tList.h"

ThreadList::ThreadList() {
	first = last = 0;
	num = 0;
}

void ThreadList::add(Thread* t) {
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

void ThreadList::remove(ID id) {
	Elem* temp = first;
	Elem* prev = 0;

	while(temp != 0 && temp->data->getId() != id) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == 0) return; // Exception, thread not found
	num--;
	if (prev != 0) {
		prev->next = temp->next;
		delete temp;
	} else {
		// Temp was the first node
		prev = temp;
		temp = temp->next;
		first = temp;
		if (first == 0) {
			last = 0;
			num = 0;
		}
		delete prev;
	}
}

Thread* ThreadList::getByID(ID id) {
	if (first == 0) return 0;
	Elem* temp = first;

	while(temp != 0 && temp->data->getId() != id) {
		temp = temp->next;
	}

	if (temp != 0) {
		return temp->data;
	}

	return 0;
}

void ThreadList::blockSignal(SignalId signal) {
	Elem* temp = first;

	while(temp != 0) {
		temp->data->blockSignal(signal);
		temp = temp->next;
	}
}

void ThreadList::unblockSignal(SignalId signal) {
	Elem* temp = first;

	while(temp != 0) {
		temp->data->unblockSignal(signal);
		temp = temp->next;
	}
}

ThreadList::~ThreadList() {}
