#include "main.h"

int main(void)
{
  uint32_t idr = 0;
  int errorCode = 0; CoreMode mode ;

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

  // Test_State state = SET_CSW_VALUE;
  //static Tlv *receivedPacket;
  while(1)
  {
    tlvService(session);
    probeTaskManager(session);
    //receivedPacket = tlvReceive(session);
    //if(verifyTlvPacket(receivedPacket)) {
    	//tlvSend(session, receivedPacket);
    //}
  }
}
