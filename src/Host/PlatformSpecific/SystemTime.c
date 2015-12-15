#include "SystemTime.h"

uint16_t getSystemTime(void) {
  static SYSTEMTIME st;
  GetSystemTime(&st);
  
  return (st.wSecond * 1000) + st.wMilliseconds;
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