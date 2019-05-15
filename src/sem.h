#ifndef SEM_H_
#define SEM_H_

typedef unsigned int Time;

class KernelSem;

class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore();

	virtual int wait(Time maxTimeToWait);
	virtual int signal(int n=0);

	int val () const;

private:
	friend class KernelSem;
	friend class SemList;
	KernelSem* myImpl;
};



#endif /* SEM_H_ */
