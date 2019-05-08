#ifndef BSTN_H_
#define BSTN_H_
#include "thread.h"
#include <iostream.h>

class bstNode {
public:
	Thread* t;
	ID id;
	bstNode* left;
	bstNode* right;
	bstNode* parent;

	bstNode();
	bstNode(Thread*);
	void setId(ID);
	Thread* getT();
};



#endif /* BSTN_H_ */
