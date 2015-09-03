#include "main.h"

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0;
  int errorCode = 0, dummy = 0;

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

  Tlv_Session *session = tlvCreateSession();

  //uint8_t buffer[] = {0xDD, 0xCC, 0xBB, 0xAA};
  //Tlv *tlv = tlvCreatePacket(TLV_WRITE_REGISTER, sizeof(buffer), buffer);
  Tlv *received;
  //tlvSend(session, tlv);

  while(1)
  {
    tlvService(session);
    if(session->DATA_ARRIVE_FLAG == true)	{
    	received = tlvReceive(session);
    	if(received != NULL)	{
    		if(received->type == TLV_WRITE_REGISTER)
    			writeTargetRegister(session, &get4Byte(&received->value[0]), &get4Byte(&received->value[4]));
    		if(received->type == TLV_READ_REGISTER)
    			readTargetRegister(session, &get4Byte(&received->value[0]));
    	}
    }
  }
}
