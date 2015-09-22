#include "main.h"

int main(void)
{
  uint32_t idr = 0, mainStack, dataRead = 0, dataRead2 = 0, dataRead3, dataRead4;
  int errorCode = 0, dummy = 0;
  CoreMode mode ;
  uint32_t data1, data2, data3, data4;

  /* Hardware configuration */
  HAL_Init();
  configure_IOPorts();
  configureLED();
  SystemClock_Config();

  /* Hardware reset target board */
  hardResetTarget();
  /* Initialize SWD Protocol */
  errorCode = swdInit();
  /* Power Up AHB Port */
  errorCode = readAhbIDR(&idr);

  //Tlv_Session *session = tlvCreateSession();

  //if(IsTargetSvcDone()) {
	  //requestCopy(0x20000100, 0x081C0000, 16);
  //}

  memoryWriteWord(0x20000000, 0xdeadbeef);
  memoryWriteWord(0x20000004, 0xbeefcafe);
  memoryWriteWord(0x20000008, 0x12345678);
  memoryWriteWord(0x2000000C, 0xabcdabcd);

  memoryReadWord(0x20000000, &dataRead);
  memoryReadWord(0x20000004, &dataRead);
  memoryReadWord(0x20000008, &dataRead);
  memoryReadWord(0x2000000C, &dataRead);

 requestCopy(0x20000000, 0x081C0000, 16);

 requestSramAddress();

  while(1)
  {
    //tlvService(session);
    //probeTaskManager(session);
  }
}
