#include "bstT.h"

bstTree::bstTree() {
	root = 0;
	height = -1;
}

bstTree::bstTree(bstNode* rt) {
	root = rt;
	height = -1;
}

Thread* bstTree::bstFind(ID id) {
	bstNode* temp = root;
	ID tempID = temp->t->getId();
		while ((temp != 0) && (tempID != id)) {
			if (id < tempID) {
				temp = temp->left;
			}
			else if (id > tempID) {
				temp = temp->right;
			}
			else {
				return temp->t;
			}
			tempID = temp->t->getId(); // current Thread ID
		}

		return temp->t;
}

void bstTree::deleteNode(ID id) {
		bstNode* temp = root;
		if (!temp) return; // error
		bstNode* h1, *h2, *h3;
		ID tempID = temp->t->getId();
		h1 = h2 = h3 = 0;

		while (temp && id != tempID) {

			if (id < tempID) {
				temp = temp->left;
			}
			else {
				temp = temp->right;
			}
			tempID = temp->t->getId();
		}

		if (!temp) {
			return; // Exception
		}

		if (!temp->left) {
			h1 = temp->right;
		}
		else if (!temp->right) {
			h1 = temp->left;
		}
		else {
			h2 = temp;
			h1 = temp->right;
			h3 = temp->left;

			while (!h3) {
				h2 = h1;
				h1 = h3;
				h3 = h1->left;
			}

			if (h2 != temp) {
				h2->left = h1->right;
				h1->right = temp->right;
			}
			h1->left = temp->left;

		}

		if (!temp->parent) {
			root = h1;
		}
		else if (temp == temp->parent->left) {
			temp->parent->left = h1;
		}
		else {
			temp->parent->right = h1;
		}

		delete temp;
		height--;
}

void bstTree::instBst(Thread* t) {
		bstNode * temp = root;
		bstNode * prev = 0;

		while (temp != 0) {
			prev = temp;

			if (t->getId() < temp->t->getId()) {
				temp = temp->left;
			}
			else {
				temp = temp->right;
			}
		}

		if (!prev) {
			root = new bstNode(t);
			root->parent = 0;
			height++;
		}

		else {
			if (t->getId() < prev->t->getId()) {
				prev->left = new bstNode(t);
				prev->left->parent = prev;
				height++;
			}
			else {
				prev->right = new bstNode(t);
				prev->right->parent = prev;
				height++;
			}
		}
}

bstTree::~bstTree() {
		Stack S;
		bstNode* next = root;

		while (next) {
			S.push(next);
			next = next->left;
		}

		while (!S.isEmpty()) {
			next = S.pop();
			if (next->t->getId() > 0) {
				next->setId(-next->t->getId());
				S.push(next);
				next = next->right;
				while (next) {
					S.push(next);
					next = next->left;
				}
			}
			else {
				next->setId(-next->t->getId());
				delete next;
			}
		}
		root = 0;
		height = -1;
}
