#include "TLV_Protocol.h"

/**
  * tlvCalculateCheckSum is to add all the byte in buffer and then 2's compliment
  *
  * input   : buffer is a array pointer to store all the data
  *           length is to determine the total length of data
  *
  * return  : chksum is the check sum of the data in buffer
  */
int tlvCalculateCheckSum(uint8_t *buffer, int length) {
  int i;
  uint8_t sum = 0, chksum = 0, result = 0;
  
  for(i = 0; i < length; i++) {
    sum += buffer[i];
    //printf("hex %x dec %d\n", buffer[i], buffer[i]);
    //printf("sum %d\n", sum);
  }
  
  
  chksum = sum % 256;
  
  //printf("chksum modulo 256 %d", chksum);
  chksum = ~chksum + 1;
  
  return chksum;
}

/**
  * tlvCreatePacket create a packet contain all the information needed for tlv protocol
  *
  * input   : type is to determine the instruction send by pc/probe
  *           length is to determine the total length of data need to be send/receive
  *           value is the data32 to write/read
  *
  * return  : return a TLV type pointer address
  */
TLV_TypeDef *tlvCreatePacket(uint8_t type, uint8_t length, uint8_t *value) {
  uint8_t chksum = 0;
  static TLV_TypeDef tlvPacket;
  
  tlvPacket.type = type;
  tlvPacket.length = length;
  tlvPacket.value = value;
  
  return &tlvPacket;
}

/**
  * tlvPackBtyeIntoBuffer is a function to pack the created TLV_TypeDef struct pointer
  * into the buffer
  *
  * buffer :    type     length     value    checkSum
  *          +--------------------------------------+
  *          |1st byte| 2nd byte | 3rd byte|   0    |           
  *          +--------------------------------------+
  *
  * input   : buffer is a array pointer to store data that need to be transmit
  *           tlvPacket is a TLV_TypeDef struct pointer
  *
  * return  : return how many byte is packed into buffer
  */
int tlvPackPacketIntoBuffer(uint8_t *buffer, TLV_TypeDef *tlvPacket) {
  int i = 0;
  
  /* First byte of the buffer is reserved for type */
  buffer[0] = tlvPacket->type;
  /* Second byte of the buffer is reserved for type */
  buffer[1] = tlvPacket->length;
  
  for(i = 0; i < tlvPacket->length; i++) {
    /* buffer +2 is use to reserve the size for type and length */
    buffer[i+2] = tlvPacket->value[i];
  }
  
  /* check sum locate at the last byte after type, length and value */
  buffer[tlvPacket->length + 2] = tlvCalculateCheckSum(tlvPacket->value, tlvPacket->length);
  
  /* type + length + data + checksum */
  return i + 3;
}

/**
  * tlvCheckProbeStatus is a function to check probe status
  *
  * input   : buffer contain an instruction send from probe
  *
  * return  : return 1 if probe is busy
  *           return 0 if probe is ok
  */
int tlvCheckProbeStatus(HANDLE hSerial, uint8_t size, uint8_t *rxBuffer) {
  int dataSize = 0;
  
  /*
  dataSize = readFromSerialPort(hSerial, rxBuffer, size);
  
  if(rxBuffer[0] == TLV_OK_INSTRUCTION) {
    printf("Probe reply OK!\n");
    rxBuffer[0] = TLV_CLEAR_INSTRUCTION;
    return PROBE_OK;
  }
  
  else if(rxBuffer[0] == TLV_BUSY_INSTRUCTION)  {
    printf("Probe reply BUSY!\n");
    return PROBE_BUSY;
  }*/
  
  return 1;
}

/**
  * tlvHost
  */
void tlvHost(TlvState *state, HANDLE hSerial)  {
  int size = 0; TLV_TypeDef *tlv;
  uint8_t txBuffer[1024], rxBuffer[1024];
  
  uint8_t data[200] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};
  
  switch(*state)  {
    case TLV_START :
      //fread(data, size - 1, 1, pFile); // read bytes to our buffer  
      tlv = tlvCreatePacket(TLV_WRITE, sizeof(data), data);
      size = tlvPackPacketIntoBuffer(txBuffer, tlv);
      *state = TLV_TRANSMIT_DATA;
      break;
      
    case TLV_TRANSMIT_DATA :
      writeToSerialPort(hSerial, txBuffer, size);
      *state = TLV_WAIT_REPLY;
      break;
      
    case TLV_WAIT_REPLY :
      /* wait reply here */
      readFromSerialPort(hSerial, rxBuffer, ONE_BYTE);
      //do  {
        //readFromSerialPort(hSerial, rxBuffer, ONE_BYTE);
      //} while(*rxBuffer != TLV_OK);
      *state = TLV_START;
      break;
  }
}