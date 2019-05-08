#include "stack.h"

Stack::Stack() {
	size = 0;
	top = 0;
}
void Stack::push(bstNode* n) {
	Elem* temp = new Elem(n, top);
	top = temp;
	size++;
}

bstNode* Stack::pop() {
	if (top == 0) return 0;

	Elem* tempElem = top;
	top = top->next;
	tempElem->next = 0;
	size--;
	return tempElem->n;
}

int Stack::isEmpty() {
	if (size == 0) {
		return 1;
	}
	return 0;
}
