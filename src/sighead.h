#ifndef SIGHEAD_H_
#define SIGHEAD_H_

typedef void (*SignalHandler)();

class SigHead {
	struct Elem {
		SignalHandler sh;
		Elem* next;
		Elem(SignalHandler sigh) {
			sh = sigh;
			next = 0;
		}
	};

	Elem* first;
	Elem* last;
	int blockedState;
	int signalRequest;
	int num; // Number of registered Handlers
public:
	SigHead();
	void setRequest(int state);
	void runHandlers();
	void addHandler(SignalHandler sh);
	void removeHandlers();
	void swapHandlers(SignalHandler h1, SignalHandler h2);
	void changeState(int state);
	int isRequested();
	int isBlocked();
	~SigHead();
};



#endif /* SIGHEAD_H_ */
