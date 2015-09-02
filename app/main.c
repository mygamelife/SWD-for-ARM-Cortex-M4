#include "main.h"

void dummyThrow(void);
void uartSendSomething(UART_HandleTypeDef *uartHandler, uint8_t *state);
void uartReceiveSomething(UART_HandleTypeDef *uartHandler, uint8_t *state);

int main(void)
{
  uint32_t idr = 0, dataRead = 0, dataRead2 = 0;
  int errorCode = 0, dummy = 0;
  UART_STATE state = UART_START;
  UART_STATE state2 = UART_START;

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

  Tlv_Session session;
  Tlv *tlv;

  UART_HandleTypeDef *uartHandler = uartInit();
  // session.receiveState = START_RECEIVE;
  // session.sendState = END_SEND;

  while(1)
  {
    uartSendSomething(uartHandler, &state);
    uartReceiveSomething(uartHandler, &state2);
  }
}

void uartSendSomething(UART_HandleTypeDef *uartHandler, uint8_t *state)	{
  uint8_t buffer[] = {0xEF, 0xBE, 0xAD, 0xDE};
  
  switch(*state)  {
    case UART_START :
      HAL_UART_Transmit_IT(uartHandler, buffer, sizeof(buffer));
      *state = UART_WAIT;
      break;
      
    case UART_WAIT :
      /*##-3- Wait for the end of the transfer ###################################*/
      if(uartReady == SET)  {
    	uartReady = RESET;
        *state = UART_START;
      }
      break;
  }
}

void uartReceiveSomething(UART_HandleTypeDef *uartHandler, uint8_t *state)	{
  uint8_t rxBuffer[255];

  switch(*state)  {
    case UART_START :
      if(HAL_UART_Receive(uartHandler, rxBuffer, 10, FIVE_SEC) == HAL_OK) {
        *state = UART_END;
      }
      break;
      
    case UART_END :
      //do nothing
      break;
  }
}
