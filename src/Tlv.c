#include "Tlv.h"

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
  tlv.value = data;
  
  return &tlv;
}

/** tlvCreateSession
  *
  * input   : NONE
  *
  * return  : NONE
  */
Tlv_Session *tlvCreateSession(void) {
  static Tlv_Session session;
  
  session.hSerial = initSerialComm(UART_PORT, UART_BAUD_RATE);
  return &session;
}

/** tlvSend is function to send tlv packet
  *
  * input   : session contain all the information needed by the  function
  *           tlv is a Tlv structure pointer contain type, length and data
  *
  * return  : NONE
  */
void tlvSend(Tlv_Session *session, Tlv *tlv)  {
  int index;  uint8_t sum = 0, chksum = 0;
  
  /** Send first 2 bytes of Tlv packet 
    * tlv->type and tlv->length
    */
  uartSendBytes(session->hSerial, (uint8_t *)tlv, 2);
  
  /** Send tlv->value according to the tlv->length
    * and calculate chksum at the same time
    */
  for(index = 0; index < tlv->length - 1; index++)  {
    sum += tlv->value[index];
    uartSendBytes(session->hSerial, &tlv->value[index], 1);
  }
  
  /** Send tlv->value according to the tlv->length  */
  chksum = ~sum + 1;
  uartSendBytes(session->hSerial, &chksum, 1);
}

/** tlvReceive is a function to receive tlv packet
  *
  * input   : session contain all the information needed by the  function
  *
  * return  : NONE
  */
Tlv *tlvReceive(Tlv_Session *session) {
  static Tlv tlv; int time = 100;
  
  /** wait for a short time period and throw
      an exception if time expired */
  do  {
    /** Waiting first 2 byte of tlv packet to arrive */
    if(time == 0) {
      Throw(ERR_TIME_OUT);
    }
    else if(uartGetBytes(session->hSerial, session->rxBuffer, 2) != 0) break;
  } while(time-- != 0);
  
  tlv.type = session->rxBuffer[0];
  tlv.length = session->rxBuffer[1];

  /** Retrieve data from buffer only length is greater than one */
  if(tlv.length > 1) {
    uartGetBytes(session->hSerial, session->rxBuffer, tlv.length);
    tlv.value = session->rxBuffer;
  }
  
  return &tlv;
}

