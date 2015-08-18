#include "main.h"

int main(void)
{
  Probe_TypeDef probe;
  uint32_t idr = 0, dataRead = 0, ack = 0;

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
  probe.uartHandle = initUart();

  setCoreMode(CORE_DEBUG_HALT);
  while(probe.state != PROBE_END)	{
	  probeProgrammer(&probe);
  }
  writeCoreRegister(CORE_REG_PC, 0x20000000);
  setCoreMode(CORE_NORMAL_MODE);
  memoryReadWord(0x20000000, &dataRead);
  memoryReadWord(0x20000004, &dataRead);
  memoryReadWord(0x20000008, &dataRead);
  memoryReadWord(0x2000000C, &dataRead);
  memoryReadWord(0x20000010, &dataRead);
  memoryReadWord(0x2000001C, &dataRead);

  // memoryWriteWord(0x200051E0, 0xDEADBEEF);
  // memoryWriteWord(0x200051E4, 0xDEADBEEF);
  // memoryWriteWord(0x200051E8, 0xDEADBEEF);
  // memoryWriteWord(0x200051EC, 0xDEADBEEF);
  // loadCopyFromSRAMToFlashInstruction(0x200051E0, 0x081C0000, 16);


  while(1)
  { }
}
