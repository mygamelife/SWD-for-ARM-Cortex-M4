#include "Tlv.h"

/** tlvPackIntoBuffer is a function to pack currentBuffer into targetBuffer
  */
void tlvPackIntoBuffer(uint8_t *targetBuffer, uint8_t *currentBuffer, int length) {
  int index = 0;
  uint8_t chksum = 0;
  
  for(index; index < length; index++) {
    chksum += targetBuffer[index] = currentBuffer[index];
  }
  targetBuffer[index] = ~chksum + 1;
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
Tlv *tlvCreatePacket(uint8_t command, int size, uint8_t *data) {
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
  session->SEND_DATA_FLAG = true;
  
  session->txBuffer[0] = tlv->type;
  session->txBuffer[1] = tlv->length;
  tlvPackIntoBuffer(&session->txBuffer[2], tlv->value, tlv->length);
}

/** tlvSendService is a state machine to handle the tlvSend
  *
  * input   : session contain all the information needed by the function
  *
  * return  : NONE
  */
#ifndef HOST
void tlvSendService(Tlv_Session *session) {
  int length = 0;
  
  // printf("hi\n");
  if(session->SEND_DATA_FLAG == true) {
    length = session->txBuffer[1] + 2;
    sendBytes(session->handler, session->txBuffer, length);
    session->SEND_DATA_FLAG = false;
  }
}

#else
void tlvSendService(Tlv_Session *session)	{
  switch(session->sendState)  {
    case START_SEND :
      if(session->SEND_DATA_FLAG == true) {
        if(uartReady == SET)  {
          sendBytes(session->handler, session->txBuffer, length);
          uartReady = RESET;
          session->SEND_DATA_FLAG = false;
        }    
      }
      break;
  }
}
#endif

/** tlvReceive is a function to receive tlv packet
  *
  * input   : session contain all the information needed by the  function
  *
  * return  : tlv structure
  */
Tlv *tlvReceive(Tlv_Session *session) {
  static Tlv tlv;

  if(session->DATA_ARRIVE_FLAG == false)  return NULL;

  tlv.type = session->rxBuffer[0];
  tlv.length = session->rxBuffer[1];
  tlvPackIntoBuffer(tlv.value, &session->rxBuffer[2], tlv.length);
  
  return &tlv;
}

/** tlvReceiveService is a state machine to handle the tlvReceive
  *
  * input   : session contain all the information needed by the function
  *
  * return  : NONE
  */
void tlvReceiveService(Tlv_Session *session) {
  session->TIMEOUT_FLAG = false;
  session->DATA_ARRIVE_FLAG = false;
  int length = 0;
  
  switch(session->receiveState)  {
    case START_RECEIVE :
      if(!getBytes(session->handler, session->rxBuffer, 2))  {
        /* set DATA_ARRIVE_FLAG when packet is arrived */
        session->DATA_ARRIVE_FLAG = true;
        length = session->rxBuffer[1];
        if(getBytes(session->handler, &session->rxBuffer[2], length))	{
          /* set TIMEOUT_FLAG when timeout occur */
        	session->TIMEOUT_FLAG = true;
        }
      }
      break;
  }
}

/** tlvService is a function routine to handle asychronize send and synchronize 
  * receive for tlv protocol
  */
void tlvService(Tlv_Session *session) {
  tlvSendService(session);
  tlvReceiveService(session);
}

/** verifyTlvData is a function to verify the data inside 
  * tlv packet by sum up all the data
  *
  * input   : tlv is pointer pointing tlv packet
  *
  * return  : DATA_VALID data is correct
  *           DATA_INVALID data is corrupted
  */
int tlvVerifyData(Tlv *tlv) {
  int i; uint8_t result = 0;
  
  for(i = 0; i < tlv->length; i++)
    result += tlv->value[i];
  
  if(result == 0)
    return DATA_VALID;
  
  else return DATA_INVALID;
}
