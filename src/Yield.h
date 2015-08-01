#ifndef Yield_H
#define Yield_H

#define startTask(x)              switch((x)->state)	{ case 0:
#define yield(x)                  (x)->state = __LINE__; return ; case __LINE__:;
#define endTask(x)                }

#endif // Yield_H
