#ifndef Yield_H
#define Yield_H

typedef struct
{
  int state;
} TaskBlock;

extern int isYielding;

#define resetTask(x)              (x)->state = 0;
#define startTask(x)              switch((x)->state) { case 0 :
#define yield(x)                  do {(x)->state = __LINE__; isYielding = 1; return 0; case __LINE__:;} while(0)
#define endTask(x)                (x)->state = 0; }

#define await(result, func, x)    do {case __LINE__:; (x)->state = __LINE__; (result) = (func); if(isYielding) return 0;} while(0)
#define returnThis(result, x)     do {isYielding = 0; (x)->state = 0; return result;} while(0)
  
#endif // Yield_H
