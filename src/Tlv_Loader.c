#include "Tlv_Loader.h"

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
Tlv_Session *tlvCreateLoaderSession(void) {
  static Tlv_Session session;
  session.handler = initSerialComm(UART_PORT, UART_BAUD_RATE);
  
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
  HANDLE hSerial = (HANDLE)session->handler;
  uartSendBytes(hSerial, (uint8_t *)tlv, 2);
  
  /** Send tlv->value according to the tlv->length
    * and calculate chksum at the same time */
  uartSendBytes(hSerial, tlv->value, tlv->length);
}

/** tlvReceive is a function to receive tlv packet
  *
  * input   : session contain all the information needed by the  function
  *
  * return  : tlv structure
  */
Tlv *tlvReceive(Tlv_Session *session) {
  static Tlv tlv; int time = 100;
  HANDLE hSerial = (HANDLE)session->handler;
  
  /** wait for a short time period and throw
      an exception if time expired */
  do  {
    if(time == 0) {
      Throw(ERR_TIME_OUT);
    }
    /** Waiting first 2 byte of tlv packet to arrive */
    else if(uartGetBytes(hSerial, session->rxBuffer, 2) != 0) {
      tlv.type = session->rxBuffer[0];
      tlv.length = session->rxBuffer[1];
      break;
    }
  } while(time-- != 0);
  
  /** Retrieve tlv data from buffer according to the tlv->length*/
  uartGetBytes(hSerial, session->rxBuffer, tlv.length);
  tlvPackIntoBuffer(tlv.value, session->rxBuffer, tlv.length);

  return &tlv;
}