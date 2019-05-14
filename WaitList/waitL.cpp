#include "waitL.h"
#include <iostream>
using namespace std;
WaitList::WaitList() {
	first = 0;
	numOfWaiting = 0;
}

void WaitList::addWaiting(/*Thread* t,*/ int val) {
	if (first == 0) {
		// List is empty
		first = new Elem(/*t,*/ val);
	}
	else {
		// The list has some items
		Elem* temp = first;
		int sum = temp->val;

		while (temp->next != 0 && sum <= val) {
			temp = temp->next;
			sum += temp->val;
		}

		int newVal = 0;
		Elem* tempElem = 0;
		if (val <= sum) {
			newVal = val - (sum - temp->val);
		}
		else {
			newVal = val - sum;
		}
		
		tempElem = new Elem(/*t, */newVal);

		if (val <= sum) {
			// Place it to the left of temp
			
			temp->val -= newVal;

			tempElem->next = temp;
			tempElem->prev = temp->prev;
			if (temp->prev) {
				temp->prev->next = tempElem;
			}

			temp->prev = tempElem;
			if (temp == first) {
				first = tempElem;
			}
		}
		else {
			// Place it to the right of temp
			tempElem->prev = temp;
			tempElem->next = temp->next;

			temp->next = tempElem;
			if (tempElem->next) {
				tempElem->next->prev = tempElem;
				tempElem->next->val -= newVal;
			}
		}
	}

	numOfWaiting++;
	printList();
}

void WaitList::removeWaiting() {
	if (numOfWaiting == 0) return;
	first->val--;

	if (first->val == 0) {
		Elem* temp = first;
		Elem* prev = 0;

		while (temp && temp->val == 0) {
			prev = temp;
			temp = temp->next;
			if (temp != 0) temp->prev = 0;

			//prev->t->myPCB->setState(PCB::Ready);
			//Scheduler::put(prev->t->myPCB);

			prev->next = 0;
			delete prev;

			numOfWaiting--;
		}
		first = temp;
	}
	printList();
}

int WaitList::getNumOfWaiting() const {
	return numOfWaiting;
}

void WaitList::printList() {
	Elem* temp = first;
	cout << "Num of waiting: " << numOfWaiting << endl;
	while (temp) {		
		cout << temp->val << " ";
		temp = temp->next;
	}
}