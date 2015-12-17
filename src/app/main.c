#include "main.h"

int main(void)
{
  CEXCEPTION_T err;
  uint32_t idr = 0;

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
  /*Initialise FPB Unit*/
  initialiseFPBUnit();

  Tlv_Session *session = tlvCreateSession();
  
  // swoInit(&session->hswo);
  // getSwoBytes(session->hswo, session->swoBuffer, 5);
  // itmInit();
  // memoryWriteWord((uint32_t)&ITM->PORT[0].u32, 0xDEADBEEF);
  //memoryWriteWord((uint32_t)&ITM->PORT[0].u32, 0x0000FACE);
  //memoryWriteWord((uint32_t)&ITM->PORT[0].u32, 0x000000AA);

  while(1)
  {
    Try {
      tlvService(session);
      taskManager(session);
    }
    Catch(err) {
      tlvErrorReporter(session, err);
    }
  }
}

