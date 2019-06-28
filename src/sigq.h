#ifndef SIGQ_H_
#define SIGQ_H_

typedef unsigned SignalId;

class SigQueue {
	struct SigElem {
		SignalId num;
		SigElem* next;
		SigElem(SignalId num) {
			this->num = num;
			next = 0;
		}
	};

	SigElem* first;
	SigElem* last;
	int size;
public:
	SigQueue();
	void addElem(SignalId);
	SignalId getElem();
	int getSize();
	~SigQueue();

};


#endif /* SIGQ_H_ */
