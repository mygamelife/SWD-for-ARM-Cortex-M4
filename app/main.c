#include "main.h"

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0;
  int errorCode = 0;

  /* Hardware configuration */
  HAL_Init();
  configureUartPorts();
  configure_IOPorts();
  SystemClock_Config();

  /* Hardware reset target board */
  hardResetTarget();
  /* Initialize SWD Protocol */
  errorCode = swdInit();
  /* Power Up AHB Port */
  errorCode = readAhbIDR(&idr);

  Tlv_Session *session = tlvCreateWorkerSession();

  while(1)
  {
	  programWorker(session);
  }
}
