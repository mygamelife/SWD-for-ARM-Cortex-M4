#include "main.h"

void dummyThrow(void);

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0;
  int errorCode = 0, dummy = 0;

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

  Tlv_Session session;
  Tlv *tlv;

  session.handler = uartInit();
  session.receiveState = START_RECEIVE;
  session.sendState = END_SEND;

  while(1)
  {
	  tlvService(&session);
	  tlv = tlvReceive(&session);
	  if(tlv != NULL)
		  tlvSend(&session, tlv);
  }
}
