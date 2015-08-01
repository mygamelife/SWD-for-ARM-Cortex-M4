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
  }
  
  chksum = sum % 256;
  
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
  * return  : NONE
  */
void tlvPackPacketIntoTxBuffer(uint8_t *buffer, TLV_TypeDef *tlvPacket) {
  int i = 0, index = 0;
  
  /* First byte of the buffer is reserved for type */
  buffer[index++] = tlvPacket->type;

  /** Second byte of the buffer is reserved for length
    * Length need to reserve extra 3 spaces for TLV_Protocol type, length and checksum
    * Therefore is the tlvPacket->length + type + length + checksum
    */
  buffer[index++] = tlvPacket->length + 3;

  for(i = 0; i < tlvPacket->length; i++) {
    buffer[index] = tlvPacket->value[i];
    index++;
  }
  
  /* check sum locate at the last byte after type, length and value */
  buffer[index] = tlvCalculateCheckSum(tlvPacket->value, tlvPacket->length);
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
  //printf("byteData %x\n", byteData);
  
  return byteData;
}

/** tlvCreateDataBuffer is a function to create dataBuffer to store information
  *
  * input     : buffer is the array use to store data information
  *             size is the size of the buffer
  *
  * return    : tlvDataBuffer is a TLV_DataBuffer type structure contain data and length
  */
TLV_DataBuffer *tlvCreateDataBuffer(uint8_t *buffer, int size)  {
  static TLV_DataBuffer tlvDataBuffer;
  
  tlvDataBuffer.data = buffer;
  tlvDataBuffer.length = size;
  
  return &tlvDataBuffer;
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
  */
void tlvPutDataIntoBuffer(TLV_DataBuffer *dataBuffer, ElfSection *pElf) {
  int i = 0, length = dataBuffer->length;
  
  tlvGetBytesAddress(pElf->address, dataBuffer->data);
  
  for(i; i < length; i++) {
    /* data start at position four due to the first 4 position is reserved for address */
    dataBuffer->data[i + 4] = tlvGetByte(pElf->machineCode, i);
  }
  
  /* Updata elf section address and size */
  pElf->address = pElf->address + length;
  pElf->size = pElf->size - dataBuffer->length;
}
#endif
