#ifndef Yield_H
#define Yield_H

typedef struct
{
  int state ;
}TaskBlock;

#define initTaskBlock(x)	        (x)->state = 0 ;
#define startTask(x)              switch((x)->state)	{ case 0:
#define yield(x)                  (x)->state = __LINE__; return ; case __LINE__:;
#define endTask(x)                }

#endif // Yield_H
