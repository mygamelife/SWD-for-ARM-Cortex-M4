#include "TLV_Host.h"

/**
  * tlvCreatePacket create a packet contain all the information needed for tlv protocol
  *
  * packet :    type          length    Address     value    chksum
  *          +--------------------------------------------------------+
  *          |  1st byte  |  2nd byte  |       |  3rd byte  |   0    |           
  *          +-------------------------------------------------------+
  *
  * input   : type is to determine the instruction send by pc/probe
  *           length is to determine the total length of data need to be send/receive
  *           value is the data32 to write/read
  *
  * return  : return a TLV type pointer address
  */
TLV *tlvCreateNewPacket(uint8_t type) {
  int index = 0;
  static TLV tlvPacket;
  
  tlvPacket.type = type;
  tlvPacket.length = 0;
  
  for(index = 0; index < DATA_SIZE; index++)
    tlvPacket.value[index] = 0;
  
  return &tlvPacket;
}

/**
  * tlvCalculateCheckSum is to add all the byte in buffer and then 2's compliment
  *
  * input   : buffer is a array pointer to store all the data
  *           length is to determine the total length of data
  *
  * return  : chksum is the check sum of the data in buffer
  */
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index) {
  uint8_t sum = 0;
  
  for(index; index < length; index++) {
    sum += buffer[index];
  }
  
  return ~sum + 1;
}

/**
  * tlvVerifyCheckSum
  *
  * input   : buffer is a array pointer to store all the data
  *           length is to determine the total length of data
  *           index is value start position
  *           chksum is actual check sum value
  *
  * return  : 1   correct
  *           0   wrong
  */
uint8_t tlvVerifyCheckSum(uint8_t *buffer, int length, int index) {
  uint8_t sum = 0, result = 0, chksum = 0;
  
  length = length - CHECKSUM_LENGTH;

  for(index; index < length; index++) {
    sum += buffer[index];
  }

  chksum = buffer[length];
  result = sum + chksum;
  
  if(result == 0)
    return 1;
  
  else 0;
}

/** tlvPackBytesAddress is a function convert 32bit address to bytes and pack into buffer
  *
  * input     : address is the 32bit address need to be split
  *             buffer is the place to store bytes address
  *             index is the start position to pack
  * 
  * return    : NONE
  */
void tlvPackBytesAddress(uint32_t address, uint8_t *buffer, int index)  {
  buffer[index]     = (address & 0xff000000) >> 24;
  buffer[index + 1] = (address & 0x00ff0000) >> 16;
  buffer[index + 2] = (address & 0x0000ff00) >> 8;
  buffer[index + 3] = (address & 0x000000ff) >> 0;
}

/**
  * tlvPackBtyeIntoBuffer is a function to pack the created TLV struct pointer
  * into the buffer
  *
  * buffer :    type     length    Address       value        checkSum
  *          +-------------------------------------------------------+
  *          | 1 byte  | 1 byte | 4 byte  | Multiple bytes  | 1 byte |           
  *          +-------------------------------------------------------+
  *
  * input   : buffer is a array pointer to store data that need to be transmit
  *           tlvPacket is a TLV struct pointer
  *
  * return  : NONE
  */
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV *tlvPacket) {
  int i = 0, index = 0;
  
  /* First byte of the buffer is reserved for type */
  buffer[index++] = tlvPacket->type;

  /** Second byte of the buffer is reserved for length
    */
  buffer[index++] = tlvPacket->length;

  for(i = 0; i < tlvPacket->length; i++) {
    buffer[index] = tlvPacket->value[i];
    index++;
  }
}

/** tlvGetByteDataFromElfFile is a function read the elf file and return a byte data
  *
  * input   : pElf is a ElfSection type pointer contain all the elf file information
  *
  * return  : data in bytes
  */
uint8_t tlvGetByteDataFromElfFile(ElfSection *pElf)  {

  /* Updata elf section address and size */
  pElf->address++;
  pElf->size--;
  
  return pElf->code[pElf->codeIndex++];
}

/** tlvGetDataFromElf is a function to get all the needed data from elf file 
  *
  * DATA :     Address             value              checkSum
  *          +------------------------------------------------+
  *          | 4 byte |        Multiple bytes        | 1 byte |           
  *          +------------------------------------------------+
  *
  * input   :   tlv is a TLV structure pointer contain all the Tlv info
  *             pElf is ElfSection structure pointer contain all the section info
  *
  * return  :   when elf data is finish transfer over
  */
void tlvGetDataFromElf(TLV *tlv, ElfSection *pElf)  {
  int index = 0, length;
  
  /* First 4 byte is reserved for elf address */
  tlvPackBytesAddress(pElf->address, tlv->value, 0);
  
  /** Length included address and checksum
    */
  tlv->length += ADDRESS_LENGTH + CHECKSUM_LENGTH;
  
  for(index += ADDRESS_LENGTH; index < DATA_SIZE - 1; index++) {
    
    if(pElf->size == 0) {
      tlv->value[tlv->length - 1] = tlvCalculateCheckSum(tlv->value, tlv->length - CHECKSUM_LENGTH, ADDRESS_LENGTH);
      printf("No data in elf section\n");
      return;
    }
    
    /* Retrieve information from elf file */
    tlv->value[index] = tlvGetByteDataFromElfFile(pElf);
    tlv->length++;
  }
  
  /* Checksum locate at the last position */
  tlv->value[tlv->length - 1] = tlvCalculateCheckSum(tlv->value, tlv->length - CHECKSUM_LENGTH, ADDRESS_LENGTH);
}

/** tlvHost is state machine for tlv host transmitter
  * 
  */
void tlvHost(TLVSession *tlvSession)  {
  TLV *tlv;
  uint8_t bufferState;
  uint8_t txBuffer[1024], rxBuffer = 0;
  
  switch(tlvSession->state)  {
    case TLV_START :
      /* Create new TLV packet */
      tlv = tlvCreateNewPacket(TLV_WRITE);
      
      /* Get data from elf file */
      tlvGetDataFromElf(tlv, tlvSession->pElf);
  
      /* Pack into TXBUFFER */
      if(tlvSession->pElf->size != 0) {
        tlvPackPacketIntoTxBuffer(txBuffer, tlv);
      }
      else  {
        //something here
      }
      tlvSession->state = TLV_TRANSMIT_DATA;
      break;
      
    case TLV_TRANSMIT_DATA :
      /* Transmit all data inside txBuffer to probe */
      serialWriteByte(tlvSession->hSerial, txBuffer, sizeof(txBuffer));
      tlvSession->state = TLV_WAIT_REPLY;
      break;
      
    case TLV_WAIT_REPLY :
      while(rxBuffer != PROBE_OK) {
        rxBuffer = serialGetByte(tlvSession->hSerial);
      }
      
      tlvSession->state = TLV_START;
      break;
  }
}