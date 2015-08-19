#include "main.h"

int main(void)
{
  Probe_TypeDef probe;
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0, ack = 0;

  /* Hardware configuration */
  HAL_Init();
  configureUartPorts();
  configure_IOPorts();
  SystemClock_Config();

  /* Hardware reset target board */
  resetTarget();
  /* Initialize SWD Protocol */
  SWD_Initialisation();
  /* Power Up AHB Port */
  readAhbIDR(&idr);
  swdWriteCSW(&ack, CSW_DEFAULT_MASK | CSW_WORD_SIZE);

  probe.state = PROBE_WAIT;
  TLV_Session *session = createTlvSession();
  //probe.uartHandle = initUart();

  //setCoreMode(CORE_DEBUG_HALT);
  //writeCoreRegister(CORE_REG_PC, 0x20000001);
  //setCoreMode(CORE_NORMAL_MODE);

  //readCoreRegister(CORE_REG_PC, &dataRead);
  //setCoreMode(CORE_NORMAL_MODE);

  //readCoreRegister(CORE_REG_PC, &dataRead);
  //setCoreMode(CORE_NORMAL_MODE);

  while(1)
  {
	  probeProgrammer(&probe, session);

  }
}
