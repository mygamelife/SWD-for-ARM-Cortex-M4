#include "main.h"

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0;
  int ack = 0;

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

  Tlv *receive;
  Tlv_Session *session = tlvCreateWorkerSession();
  uint32_t address, data;
  while(1)
  {
	  receive = tlvReceive(session);
	  if(receive != NULL)	{
		  if(receive->type == TLV_WRITE_REGISTER) {
			  address = get4Byte(&receive->value[0]);
			  data = get4Byte(&receive->value[4]);

			  writeTargetRegister(session, &address, &data);
		  }
		  else if(receive->type == TLV_READ_REGISTER) {
			  address = get4Byte(&receive->value[0]);
			  readTargetRegister(session, &address);
		  }
	  }
  }
}
