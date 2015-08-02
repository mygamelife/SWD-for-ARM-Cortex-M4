#include "TLV_Protocol.h"

/**
  * tlvCalculateCheckSum is to add all the byte in buffer and then 2's compliment
  *
  * input   : buffer is a array pointer to store all the data
  *           length is to determine the total length of data
  *
  * return  : chksum is the check sum of the data in buffer
  */
uint8_t tlvCalculateCheckSum(uint8_t *buffer, int length, int index) {
  int i;
  uint8_t sum = 0, chksum = 0, result = 0;
  
  for(i = index; i < length; i++) {
    sum += buffer[i];
  }
  
  chksum = sum % 256;
  
  chksum = ~chksum + 1;
  
  return chksum;
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
  int i = 0;
  uint8_t sum = 0, result = 0, chksum = 0;
  
  for(i = index; i < length - 1; i++) {
    sum += buffer[i];
  }

  chksum = buffer[length - 1];
  result = sum + chksum;
  
  if(result == 0)
    return 1;
  
  else 0;
}

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
TLV_TypeDef *tlvCreateNewPacket(uint8_t type) {
  int index = 0;
  static TLV_TypeDef tlvPacket;
  
  tlvPacket.type = type;
  tlvPacket.length = 0;
  
  for(index = 0; index < DATA_SIZE; index++)
    tlvPacket.value[index] = 0;
  
  return &tlvPacket;
}

/**
  * tlvPackBtyeIntoBuffer is a function to pack the created TLV_TypeDef struct pointer
  * into the buffer
  *
  * buffer :    type     length    Address     value    checkSum7
  *          +--------------------------------------------------+
  *          |1st byte| 2nd byte |       |  3rd byte  |   0    |           
  *          +-------------------------------------------------+
  *
  * input   : buffer is a array pointer to store data that need to be transmit
  *           tlvPacket is a TLV_TypeDef struct pointer
  *
  * return  : NONE
  */
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV_TypeDef *tlvPacket) {
  int i = 0, index = 0;
  
  /* First byte of the buffer is reserved for type */
  buffer[index++] = tlvPacket->type;

  /** Second byte of the buffer is reserved for length
    * Length need to reserve extra 1 spaces for checksum
    */
  buffer[index++] = tlvPacket->length;

  for(i = 0; i < tlvPacket->length; i++) {
    buffer[index] = tlvPacket->value[i];
    index++;
  }
}

/** tlvGetByte get byte data from elf file
  *
  * input   : data is a pointer pointing to elf section address
  *           index is the position in the elf section address
  *
  * return  : byteData is data inside the elf file in byte size
  */
TLV_Byte tlvGetByte(uint8_t *data, int index) {
  uint8_t byteData = 0;

  byteData = data[index];
  
  return byteData;
}

/** tlvGetAddressInByte is a function get 32bt address and split it into bytes and store into buffer
  *
  * input     : address is the 32bit address need to be split
  *             buffer is the place to store bytes address
  * 
  * return    : addressBuffer contain the 4 bytes address
  */
void tlvGetBytesAddress(uint32_t address, uint8_t *buffer) {
  buffer[0] = (address & 0xff000000) >> 24;
  buffer[1] = (address & 0x00ff0000) >> 16;
  buffer[2] = (address & 0x0000ff00) >> 8;
  buffer[3] = (address & 0x000000ff) >> 0;
}

/** tlvGetWordAddress is a function get 32bt address from bytes
  *
  * input     : buffer contain address in bytes
  * 
  * return    : address is a 32bit address
  */
uint32_t tlvGetWordAddress(uint8_t *buffer, int index) {
  uint32_t address = 0;
  
  address |= buffer[index] << 24;
  address |= buffer[index + 1] << 16;
  address |= buffer[index + 2] << 8;
  address |= buffer[index + 3] << 0;
  
  return address;
}

#if defined (TEST)
/** tlvPutBytesIntoBuffer is a function 
  *
  * input   :
  *
  * return  :   1 indicate data is successfull put into buffer
  *             0 indicate data is fail to put into buffer due to no data in section
  */
int tlvPutDataIntoBuffer(TLV_TypeDef *tlv, ElfSection *pElf)  {
  int index = 0, length = 0;
  
  /* Put section address into first 4 bytes of the buffer */
  tlvGetBytesAddress(pElf->address, tlv->value);
  tlv->length += ADDRESS_LENGTH + CHECKSUM_LENGTH;

  for(index += 4; index < DATA_SIZE - 1; index++) {
    if(pElf->size == 0) {
      tlv->value[tlv->length - 1] = tlvCalculateCheckSum(tlv->value, tlv->length, ADDRESS_START_POSITION);
      printf("No data in elf section\n");
      return 0;
    }
    
    /* data start at position four due to the first 4 position is reserved for address */
    tlv->value[index] = tlvGetByte(pElf->machineCode, pElf->codeIndex);
    tlv->length++;
    
    /* Updata elf section address and size */
    pElf->codeIndex++;
    pElf->address++;
    pElf->size--;
  }
  
  /* Checksum locate at the last position */
  tlv->value[tlv->length - 1] = tlvCalculateCheckSum(tlv->value, tlv->length, ADDRESS_START_POSITION);
  
  return 1;    
}
#endif

/** Abort if size is 0
  *
  */
TLV_TypeDef *tlvDecodePacket(uint8_t *buffer) {
  int index = 0;
  static TLV_TypeDef tlv;
  
  tlv.type = buffer[index++];
  tlv.length = buffer[index++];
  
  tlv.address = tlvGetWordAddress(buffer, index);
  index += ADDRESS_LENGTH;
  
  // for(index; index < tlv.length - 1; index++)
    // *tlv.value++ = buffer[index];
  
  return &tlv;
}

