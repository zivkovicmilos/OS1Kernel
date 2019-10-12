![Banner](./imgs/banner.jpg)

# Project Description
OS1Kernel, as the name suggests, is a project with a goal to implement a fully functioning but rather small OS kernel. The following features were requested, each representing a large chunk of the codebase:
1. Threads
2. Semaphores
3. Events
4. Signals

# Threads
The *Thread* class has the following structure:
```c++
class Thread {
public:
    void start(); 
    void waitToComplete();  
    virtual ~Thread();
    ID getId();
    static ID getRunningId();
    static Thread * getThreadById(ID id);
protected:
    friend class PCB;
    Thread (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
    virtual void run() {}
private:
    PCB* myPCB;
};
void dispatch ();
```
## *Thread* Breakdown
* start() -> Initializes the PCB of the thread, and adds it to the Scheduler
* waitToComplete() -> A thread that calls this function on another thread will get blocked on the semaphore, until the thread for which this function was called finishes
* getId() -> Returns the unique thread id
* run() -> Actual function that the thread will execute
* dispatch() -> Synchronous context switch

The entire *Thread* class is practically a wrapper for the PCB class, that does most of the heavy-lifting. The PCB class contains all that is necessary for a thread's context to be formed, context to be switched, restored as well as signals to be handled.
One of the most important things to note here, regarding the run() method, is that there is a static wrapper function within the PCB class. The purpose of this function being static is to grab its address before the object is fully formed. The wrapper contains the call to the thread's run() method, as well as cleanup after the thread ends (like freeing semaphores, signaling, etc). The address of the wrapper function is replaced on the thread's stack, so that it continues its execution from the beginning of the run() method.

# Semaphores
```c++
class Semaphore {

public:
    Semaphore (int init=1);
    virtual ~Semaphore ();
    virtual int wait (Time maxTimeToWait);
    virtual int signal(int n=0);
    int val () const;  // Returns the current value of the semaphore
private:
    KernelSem* myImpl;
};
```
Again, as with the *Thread* class, *Semaphore* just wraps around the *KernelSem*. The implementation of the wait() and signal() functions is somewhat different from their regular functionality. 
## wait(Time maxTimeToWait)
There are 2 ways a thread can wait on a semaphore. 
1. It can be blocked indefinitely, until the semaphore is signaled. In this case, the maxTimeToWait parameter would be 0.
2. It can be blocked for a certain number of timer calls (the timer goes off every 55ms), after which it becomes unblocked and transferred to the ready queue.
## signal(int n=0)
* Signal with a value of n > 0 unblocks *n* threads waiting on that semaphore, and returns *n*.
* Signal with a value of n = 0 just increases the value of *val* by 1, and frees a thread if possible.

# Events
Events are just binary semaphores, where a thread can be blocked by waiting on an event to happen. For example, a thread can be blocked waiting on a key press by the user. After the user presses a key, the thread is unblocked.
There is nothing particularly interesting to mention here, as the implementation was standard.

# Signals
This part of the project heavily relies on the previous three things working perfectly. Signals are just a way for threads to communicate between each other. Thread A can signal thread B (for ex. b.signal(5)). The signal would be noted somewhere in B, and later executed. Of course, the body of the function for the 5th signal in this case would have to be *registered as a Handler*. When are these signals executed? The moment the thread is set to *running*, within the *timer* method just after the context switch. Signals can be blocked, swapped and their handlers added on, or removed.
For example, there could be 3 *SignalHandler*s registered for the signal 5. When someone does thread#.signal(5), all three are executed, in the order that they've been registered.

# How the Public Test worked
There are *N* producers, that concurrently fill a bounded buffer, and one consumer that grabs values from the buffer and displays them in the console. Keyboard input is also available (using Events) - when a user presses a key, it gets added into the bounded buffer, and displayed on the screen. When the user presses the ESC key, the buffer is emptied and the program exits.

# Summary
This brief description is just a simple overview of what was expected of this project for Operating Systems 1. The full project text is available in the repo, in Serbian. One of the most challenging parts was figuring out how the context switching works using the BCC compiler, and debugging the code. Most of the code was written and tested separately, and later on added into the project to work with other modules.