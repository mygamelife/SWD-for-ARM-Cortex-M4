#include "SystemTime.h"

uint16_t getSystemTime(void) {
	return HAL_GetTick()/2;
}

/** isTimeOut is a function to determine if the program 
  * running time is reached the maximum timeout time
  *
  * input   : NONE
  *
  * output  : 1 timeout occur
  *           0 maximum timeout is not reach
  */
int isTimeout(int timeout, uint32_t previousTime) {
  if((getSystemTime() - previousTime) > timeout)
    return 1;
  
  return 0;
}