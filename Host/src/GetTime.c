#include "GetTime.h"

static double pcFrequency = 0.0;
static __int64 previousTime = 0;
static __int64 currentTime;
double elapsedTime = 0.0;

void InitTimer(void) {
  LARGE_INTEGER frequency;
  
  /* Get ticks per second */
  QueryPerformanceFrequency(&frequency);

  pcFrequency = (double)(frequency.QuadPart);
}

void startTimer(void) {
  LARGE_INTEGER t1;
  
  if(pcFrequency == 0) InitTimer();
  
  QueryPerformanceCounter(&t1);
  
  previousTime = t1.QuadPart;
}

void stopTimer(void) {
  LARGE_INTEGER t2;
  
  QueryPerformanceCounter(&t2);
  
  currentTime = t2.QuadPart;
}

int getElapsedTime(void) {
  
  stopTimer();
  
  /* Compute and print the elapsed time in millisec */
  elapsedTime += ((currentTime - previousTime) * 100000 / pcFrequency);
  
  // printf("elapsedTime %fms\n", elapsedTime);
  
  if(elapsedTime > TEN_SECOND) {
    elapsedTime = 0;
    return 1;
  }
  else return 0;
}