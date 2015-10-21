#include "SystemTime.h"

static uint16_t previousTime = 0;
static uint16_t currentTime = 0;

uint16_t getSystemTime(void) {
	return HAL_GetTick()/2;
}

uint16_t getElapsedTime(void) {
  
  if(previousTime == 0) {
    previousTime = getSystemTime();
  }
  
  currentTime = getSystemTime();

  return currentTime - previousTime;
}

/** resetSystemTime is used when time out is not occur
  */
void resetSystemTime(void) {
  previousTime = 0;
  currentTime = 0;
}

/** isTimeOut is a function to determine if the program 
  * running time is reached the maximum timeout time
  *
  * input   : NONE
  *
  * output  : 1 timeout occur
  *           0 maximum timeout is not reach
  */
int isTimeOut(int timeOut) {
  
  if(getElapsedTime() > timeOut) {
    previousTime = currentTime;
    return 1;
  }
  
  else return 0;
}
