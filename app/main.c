#include "main.h"

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0, dataRead3;
  int errorCode = 0, dummy = 0;
  CoreMode mode ;

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

  readCoreRegister(CORE_REG_R0, &dataRead);
  readCoreRegister(CORE_REG_R1, &dataRead2);

  writeCoreRegister(CORE_REG_R0, 1);
  memoryWriteWord((uint32_t)&(SCB->SHCSR),0x80);
  memoryReadWord((uint32_t)&(SCB->SHCSR),&dataRead);

  setCoreMode(CORE_DEBUG_MODE);
  readCoreRegister(CORE_REG_R0, &dataRead);
  readCoreRegister(CORE_REG_R1, &dataRead2);

  //Tlv_Session *session = tlvCreateSession();

  while(1)
  {
    //tlvService(session);
    //probeTaskManager(session);
  }
}
