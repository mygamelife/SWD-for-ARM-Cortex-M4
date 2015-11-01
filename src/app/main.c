#include "main.h"

int main(void)
{
  uint32_t idr = 0;
  SwdError errorCode = 0;

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

  Tlv_Session *session = tlvCreateSession();

  while(1)
  {
    tlvService(session);
    probeTaskManager(session);
  }
}

