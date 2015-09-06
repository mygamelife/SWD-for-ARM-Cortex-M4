#include "Tlv.h"

#if defined TEST
int uartReady = 0;
#endif

/** tlvCreateSession is a function to create necessary element needed by TLV protocol */
Tlv_Session *tlvCreateSession(void) {
  static Tlv_Session session;
  
  /* get uart handler */
  #ifdef HOST//__GNUC__
  session.handler = uartInit(UART_PORT, UART_BAUD_RATE);
  #else
  session.handler = uartInit();
  #endif
  
  /* Initialize begining state for send and receive */
  session.sendState = TLV_SEND_BEGIN;
  session.receiveState = TLV_RECEIVE_BEGIN;
  
  /* Initialize all the required flag */
  session.TIMEOUT_FLAG = false;
  session.DATA_SEND_FLAG = false;
  session.DATA_RECEIVE_FLAG = false;
  session.ONGOING_PROCESS_FLAG = false;
  
  session.hostState = HOST_WAIT_USER_COMMAND;
  session.probeState = PROBE_RECEIVE_PACKET;
  
  return &session;
}

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
  session->DATA_SEND_FLAG = true;
  
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
#ifdef HOST//__GNUC__
void tlvSendService(Tlv_Session *session) {
  int length = 0;
  
  if(session->DATA_SEND_FLAG == true) {
    length = session->txBuffer[1] + 2;
    sendBytes(session->handler, session->txBuffer, length);
    session->DATA_SEND_FLAG = false;
  }
}
#else
/* uartReady is varibale used by the uart callback service routine */
void tlvSendService(Tlv_Session *session)	{
  int length = 0;
  
  switch(session->sendState)  {
    case TLV_SEND_BEGIN :
      if(session->DATA_SEND_FLAG == true) {
        if(uartReady == SET)  {
          length = session->txBuffer[1] + 2;
          sendBytes(session->handler, session->txBuffer, length);
          uartReady = RESET;
          session->DATA_SEND_FLAG = false;
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
  
  if(session->TIMEOUT_FLAG == true) {
    session->TIMEOUT_FLAG == false;
    Throw(TLV_TIME_OUT);
  }
  else if(session->DATA_RECEIVE_FLAG == false)  return NULL;

  tlv.type = session->rxBuffer[0];
  tlv.length = session->rxBuffer[1];
  tlvPackIntoBuffer(tlv.value, &session->rxBuffer[2], tlv.length);
  session->DATA_RECEIVE_FLAG = false;
  
  return &tlv;
}

/** tlvReceiveService is a state machine to handle the tlvReceive
  *
  * input   : session contain all the information needed by the function
  *
  * return  : NONE
  */
void tlvReceiveService(Tlv_Session *session) {
  int length = 0;

  switch(session->receiveState)  {
    case TLV_RECEIVE_BEGIN :
      if(!getBytes(session->handler, session->rxBuffer, 2))  {
        /* set DATA_RECEIVE_FLAG when packet is arrived */
        session->DATA_RECEIVE_FLAG = true;
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

/**
  * ==============================================================================
                            ##### Tlv Helper function #####
    ==============================================================================  
  */
  
/** verifyTlvData is a function to verify the data inside 
  * tlv packet by sum up all the data
  *
  * input   : tlv is pointer pointing tlv packet
  *
  * return  : 1 data is correct
  *           0 data is corrupted
  */
int verifyTlvData(Tlv *tlv) {
  int i; uint8_t result = 0;
  
  for(i = 0; i < tlv->length; i++)  {
    result += (uint8_t)tlv->value[i];
  }
  
  if(result == 0)
    return 1;
  
  else return 0;
}

/** isTlvAck is a function to verify tlv acknowledgement
  *
  * input   : tlv is pointer pointing tlv packet
  *
  * return  : 1 if ack
  *           0 if not ack
  */
int isTlvAck(Tlv *tlv) {
  if(tlv->length >= 1)  {
    if(tlv->type == TLV_OK) {
      return 1;
    }
    else if(tlv->type == TLV_NOT_OK)  {
      Throw(tlv->value[0]);
    }
    else return 0;
  }
}

/** verifyTlvCommand is a function to verify the tlv command
  * tlv packet by sum up all the data
  *
  * input   : command can be one of the following value :
  *               TLV_WRITE_RAM = 10,
  *               TLV_READ_RAM,
  *               TLV_WRITE_REGISTER,
  *               TLV_READ_REGISTER,
  *               TLV_HALT_TARGET,
  *               TLV_RUN_TARGET,
  *               TLV_STEP,
  *               TLV_MULTI_STEP,
  *               TLV_BREAKPOINT
  *
  * return  : 1 if command is valid
  *           0 if command is invalid
  */
int isTlvCommand(uint8_t command) {
  if(command == TLV_WRITE_RAM)              return 1;
  else if(command == TLV_READ_RAM)          return 1;
  else if(command == TLV_WRITE_REGISTER)    return 1;
  else if(command == TLV_READ_REGISTER)     return 1;
  else if(command == TLV_HALT_TARGET)       return 1;
  else if(command == TLV_RUN_TARGET)        return 1;
  else if(command == TLV_STEP)              return 1;
  else if(command == TLV_MULTI_STEP)        return 1;
  else if(command == TLV_BREAKPOINT)        return 1;
  else if(command == TLV_WRITE_RAM)         return 1;
  
  else return 0;
}

/** verifyTlvResponse is a function to verify the tlv response
  * tlv packet by sum up all the data
  *
  * input   : tlv is pointer pointing tlv packet
  *
  * return  : 1 if response is ok
  *           0 if response is NULL
  */
int verifyTlvPacket(Tlv *tlv) {
  if(tlv != NULL) {
    if(!isTlvAck(tlv)) {
      if(!isTlvCommand(tlv->type))  
        Throw(TLV_INVALID_COMMAND);
    }
    if(!verifyTlvData(tlv))
      Throw(TLV_CHECKSUM_ERROR);
    
    return 1;
  }
  else return 0;
}

/** tlvReportError is a function to create a packet
  * contain error code
  *
  * input   : errorCode is a error to represent the tlv error
  *           can be one of the TLV_ERROR_CODE value
  *
  */
void tlvReportError(Tlv_Session *session, uint8_t errorCode)  {
  Tlv *tlv = tlvCreatePacket(TLV_NOT_OK, 1, &errorCode);

  tlvSend(session, tlv);
}
