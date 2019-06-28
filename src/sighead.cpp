#ifndef SIGHEAD_CPP_
#define SIGHEAD_CPP_

#include "sighead.h";

SigHead::SigHead() {
	first = last = 0;
	signalRequest = 0;
	blockedState = 0;
	num = 0;
}

void SigHead::setRequest(int state) {
	signalRequest = state;
}

void SigHead::runHandlers() {
	if (num == 0) return; // No handlers registered
	Elem* temp = first;

	while(temp != 0) {
		SignalHandler sh = temp->sh;
		(*sh)();
		temp = temp->next;
	}
}

void SigHead::addHandler(SignalHandler sh) {
	if (first == 0) {
		first = new Elem(sh);
		last = first;
	} else {
		Elem* temp = new Elem(sh);
		last->next = temp;
		last = temp;
	}
	num++;
}

void SigHead::removeHandlers() {
	if(num == 0) return; // No handlers
	Elem* temp = first;
	Elem* prev = 0;

	while(temp != 0) {
		prev = temp;
		temp = temp->next;

		prev->sh = 0;
		prev->next = 0;
		delete prev;
	}
	first = last = 0;
	num = 0;
}

void SigHead::swapHandlers(SignalHandler h1, SignalHandler h2) {
	if (num == 0) return; // No Handlers

	Elem* firstHandler = 0;
	Elem* secondHandler = 0;

	Elem* temp = first;

	while (temp != 0 && (firstHandler == 0 || secondHandler == 0)) {
		if (temp->sh == h1) {
			firstHandler = temp;
		}
		// In case h1 == h2
		if (temp->sh == h2) {
			secondHandler = temp;
		}
		temp = temp->next;
	}

	if (firstHandler == 0 || secondHandler == 0) return; // Handlers not found

	SignalHandler tempHandler = firstHandler->sh;
	firstHandler->sh = secondHandler->sh;
	secondHandler->sh = tempHandler;

}

void SigHead::changeState(int state) {
	blockedState = state;
}

int SigHead::isRequested() {
	return signalRequest;
}

int SigHead::isBlocked() {
	return blockedState;
}

SigHead::~SigHead() {
	removeHandlers();
}



#endif /* SIGHEAD_CPP_ */
