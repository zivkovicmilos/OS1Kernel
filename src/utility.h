#ifndef UTILITY_H_
#define UTILITY_H_
#include "pcb.h"
#include "thread.h"


class Utility {
public:
	Utility();

	static void inic();
	static void restore();

	static void interrupt(*oldInterrupt)(...);
	static void interrupt timer(...);

};



#endif /* UTILITY_H_ */
