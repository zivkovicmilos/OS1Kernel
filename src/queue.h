#ifndef QUEUE_H_
#define QUEUE_H_

class PCB;

class Queue {
	struct Elem {
		PCB* data;
		Elem* next;
		Elem(PCB* d) {
			data = d;
			next = 0;
		}
	};

	Elem* first;
	Elem* last;
	int size;
public:
	Queue();
	void addElem(PCB*);
	PCB* getElem();
	int getSize();
	~Queue();
};



#endif /* QUEUE_H_ */
