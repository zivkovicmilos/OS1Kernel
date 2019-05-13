#ifndef BSTT_H_
#define BSTT_H_
#include "bstN.h";
#include "stack.h"

class bstTree {
	int height;
	bstNode* root;
public:
	bstTree(bstNode*);
	bstTree();
	Thread* bstFind(ID);
	//bool isFound(bstNode*);
	void deleteNode(ID);
	void instBst(Thread*); // Add
	~bstTree();
};



#endif /* BSTT_H_ */
