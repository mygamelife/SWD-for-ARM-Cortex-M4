#include "main.h"

int main(void)
{
  CEXCEPTION_T err;
  uint32_t idr = 0;
  //SwdError errorCode = 0;

  /* Hardware configuration */
  HAL_Init();
  configureIOPorts();
  configureLED();
  SystemClock_Config();

  /* Hardware reset target board */
  hardResetTarget();
  /* Initialize SWD Protocol */
  swdInit();
  /* Power Up AHB Port */
  readAhbIDR(&idr);

  uint32_t readData;
  int high = 0, low = 0;

  itmInit();
  memoryWriteWord((uint32_t)&ITM->PORT[0].u32, 0xDEADBEEF);
  //Tlv_Session *session = tlvCreateSession();

  while(1)
  {
    //Try {
      //tlvService(session);
      //taskManager(session);
    //}
    //Catch(err) {
      //resetSystemTime();
      //tlvErrorReporter(session, err);
    //}
	  if(readSWO()) high++;
	  else low++;

  }
}

