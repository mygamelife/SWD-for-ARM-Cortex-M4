#ifndef Yield_H
#define Yield_H

typedef struct
{
  int state ;
} TaskBlock;

#define resetTask(x)              x = 0;
#define startTask(x)              switch((x)) { case 0 :
// #define yield(x)                  (x) = __LINE__; return 0; case __LINE__:;
#define yield(x)                  (x) = __LINE__; return 0; case __LINE__:;
#define endTask(x)                x = 0; }

#endif // Yield_H
