#ifndef STACK_H_
#define STACK_H_
#include "bstN.h"

class Stack {
	struct Elem {
		bstNode* n;
		Elem* next;
		Elem(bstNode* n, Elem* next) {
			this->n = n;
			this->next = next;
		}
	};
	int size;
	Elem* top;
public:
	Stack();

	void push(bstNode*);
	bstNode* pop();
	int isEmpty();
};



#endif /* STACK_H_ */
