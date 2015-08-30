#include "Tlv_Worker.h"

/** tlvPackIntoBuffer is a function to pack currentBuffer into targetBuffer
  *
  */
void tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length) {
  int index = 0;
  uint8_t chksum = 0;
  
  for(index; index < length; index++) {
    chksum += targetBuffer[index] = currentBuffer[index];
  }
  targetBuffer[index] = ~chksum + 1;
}

/** tlvCreateSession is a function to create a useful element for TLV protocol use
  */
Tlv_Session *tlvCreateWorkerSession(void) {
  static Tlv_Session session;
  session.handler = initUart();
  
  return &session;
}

/** tlvCreatePacket create a packet contain all the information needed for tlv protocol
  *
  * packet :    type     length   Address    value    chksum
  *          +-----------------------------------------------+
  *          | 1 byte | 1 byte | 1 byte |  N bytes  | 1 byte |           
  *          +-----------------------------------------------+
  *
  * input   : type is the command to convey the meaning of this tlv packet
  *           length is the value to determine the total length of data need to be send/receive
  *           data is a pointer pointing to the data32 to write/read
  *
  * return  : return a TLV type pointer address
  */
Tlv *tlvCreatePacket(uint8_t command, uint8_t size, uint8_t *data) {
  static Tlv tlv;
  
  tlv.type = command;
  tlv.length = size + 1; //extra length for chksum
  tlvPackIntoBuffer(tlv.value, data, size);
  
  return &tlv;
}

/** tlvSend is function to send tlv packet
  *
  * input   : session contain all the information needed by the  function
  *           tlv is a Tlv structure pointer contain type, length and data
  *
  * return  : NONE
  */
void tlvSend(Tlv_Session *session, Tlv *tlv)  {
  /** Send first 2 bytes of Tlv packet
    * tlv->type and tlv->length */
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)session->handler;
  HAL_UART_Transmit(uartHandle, (uint8_t *)tlv, 2, FIVE_SEC);

  /** Send tlv->value according to the tlv->length
    * and calculate chksum at the same time */
  HAL_UART_Transmit(uartHandle, tlv->value, tlv->length, FIVE_SEC);
}

/** tlvReceive is a function to receive tlv packet
  *
  * input   : session contain all the information needed by the  function
  *
  * return  : tlv structure
  */
Tlv *tlvReceive(Tlv_Session *session) {
  static Tlv tlv;
  UART_HandleTypeDef *uartHandle = (UART_HandleTypeDef *)session->handler;
  
  /** return NULL if no data is arrive */
  if(HAL_UART_Receive(uartHandle, session->rxBuffer, 2, FIVE_SEC) != HAL_OK)  {
    return NULL;
  }
  tlv.type = session->rxBuffer[0];
  tlv.length = session->rxBuffer[1];

  /** Retrieve data from buffer only length is greater than one */
  HAL_UART_Receive(uartHandle, session->rxBuffer, tlv.length, FIVE_SEC);
  tlvPackIntoBuffer(tlv.value, session->rxBuffer, tlv.length);
  
  return &tlv;
}
