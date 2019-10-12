#include "sigq.h"


SigQueue::SigQueue() {
	first = 0;
	last = 0;
	size = 0;
}

void SigQueue::addElem(SignalId num) {
	if (first == 0) {
		first = new SigElem(num);
		last = first;
	} else {
		SigElem* temp = new SigElem(num);
		last->next = temp;
		last = temp;
	}
	size++;
}

SignalId SigQueue::getElem() {
	SignalId ret = 0;
	if (first->next != 0) {
		ret = first->num;
		SigElem* temp = first;
		first = first->next;
		delete temp;
	} else {
		ret = first->num;
		delete first;
		first = last = 0;
	}
	size--;
	return ret;
}

int SigQueue::getSize() {
	return size;
}

SigQueue::~SigQueue() {
	SigElem* temp = first;
	SigElem* prev = 0;
	while (temp != 0) {
		prev = temp;
		temp = temp->next;
		delete prev;
	}
	first = last = 0;
	size = 0;
}
