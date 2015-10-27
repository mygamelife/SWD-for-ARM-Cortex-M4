#include "main.h"

int main(void)
{
  uint32_t idr = 0;
  SwdError errorCode = 0;
  uint32_t data = 0;
  /* Hardware configuration */
  HAL_Init();
  configureIOPorts();
  configureLED();
  SystemClock_Config();

  /* Hardware reset target board */
  hardResetTarget();
  /* Initialize SWD Protocol */
  errorCode = swdInit();
  /* Power Up AHB Port */
  errorCode = readAhbIDR(&idr);

  //Tlv_Session *session = tlvCreateSession();

  uint8_t data0, data1, data2, data3;
  memoryWriteWord(0x20000004, 0xDEADBEEF);
  memoryWriteWord(0x20000008, 0xABCD1234);
  memoryReadHalfword(0x20000004, &data);
  memoryReadHalfword(0x20000005, &data);
  memoryReadHalfword(0x20000006, &data);
  memoryReadHalfword(0x20000007, &data);

  while(1)
  {
    //tlvService(session);
    //probeTaskManager(session);
  }
}

