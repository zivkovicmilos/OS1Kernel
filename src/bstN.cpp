#include "bstN.h"

bstNode::bstNode() {
	t = 0;
}

bstNode::bstNode(Thread* t) {
	this->t = t;
	this->id = t->getId();
}

void bstNode::setId(ID id) {
	this->id = id;
}
