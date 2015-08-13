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
  tlvPacket.length = 1; //chksum
  
  for(index = 0; index < TLV_DATA_SIZE; index++)
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

/** tlvPack4ByteAddress is a function convert 32bit address to bytes and pack into buffer
  *
  * input     : address is the 32bit address need to be split
  *             tlv is the pointer pointing to TLV structure
  * 
  * return    : NONE
  */
void tlvPack4ByteAddress(uint32_t address, TLV *tlv)  {
  tlv->value[0] = (address & 0xff000000) >> 24;
  tlv->value[1] = (address & 0x00ff0000) >> 16;
  tlv->value[2] = (address & 0x0000ff00) >> 8;
  tlv->value[3] = (address & 0x000000ff) >> 0;
  
  tlv->length += 4;
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
void tlvPackPacketIntoBuffer(uint8_t *buffer, TLV *tlvPacket) {
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

/** tlvGetBytesData is a function to get bytes data from elf file 
  *
  * DATA :     Address             value              checkSum
  *          +------------------------------------------------+
  *          | 4 byte |        Multiple bytes        | 1 byte |           
  *          +------------------------------------------------+
  *
  * input   :   dataAddress is the address where is data retrieve from
  *             tlv is a TLV structure pointer contain all the Tlv info
  *             size is the size of data want to retrieve
  *
  * return  :   NONE
  */
void tlvGetBytesData(uint8_t *dataAddress, TLV *tlv, int size) {
  int i;  uint8_t sum = 0;

  for(i = 0; i < size; i++) {
    tlv->value[i + ADDRESS_LENGTH] = dataAddress[i];
    sum += dataAddress[i];
    tlv->length++;
  }
  
  /** Insert cheksum  at last position */
  tlv->value[tlv->length - 1] = ~sum + 1;
}

/** <! For Internal User Only !>
  * tlvWriteDataChunk is a function to get number of data arccoding to the size and send
  * data using UART
  *
  * input   : dataAddress is the address of data 
  *           destAddress is the destination address of the data
  *           size is the size of data to transfer (TLV_DATA_SIZE   248)
  *           hSerial is used by initSerialPort() to initialize the USB-Serial port
  *
  * return  : NONE
  */
void tlvWriteDataChunk(uint8_t *dataAddress, uint32_t *destAddress, int size, HANDLE hSerial)  {
  uint8_t txBuffer[1024] = {0};
  
  TLV *tlv = tlvCreateNewPacket(TLV_WRITE);
  
  /* Pack destAddress into tlv->value */
  tlvPack4ByteAddress((uint32_t)destAddress, tlv);
  /* Get bytes data from elf file */
  tlvGetBytesData(dataAddress, tlv, size);
  /* pack tlv packet into txBuffer */
  tlvPackPacketIntoBuffer(txBuffer, tlv);
  
  uartSendBytes(hSerial, txBuffer, sizeof(txBuffer));
}

/** tlvCheckDataSize is to determine the current data file size
  * and adjust the data size to retrieve
  *
  * input     : size is the size of data can be any value
  *
  * return    : size is the remaining data size
  *             TLV_DATA_SIZE is the default data retrieve size
  */
int tlvCheckDataSize(int size)  {
  if(size < TLV_DATA_SIZE){
    return size;
  }
  
  return TLV_DATA_SIZE;
}


/** tlvWaitProbeReply is a function to receive probe reply
  *
  * input     : size is the size of data can be any value
  *
  * return    : size is the remaining data size
  *             TLV_DATA_SIZE is the default data retrieve size
  */
uint8_t tlvProbeReply() {
  
}

/** tlvWriteRam is a function to write data from elf to sram
  *
  * input     : dataAddress is the address where the data is located
  *             destAddress is the destination address of the data where should it be located
  *             size is the size of data to transfer, and can be any value
  *
  * return    : NONE
  */
void tlvWriteRam(uint8_t *dataAddress, uint32_t *destAddress, int size)  {
  uint8_t txBuffer[] = {TLV_START}, ack = 0;
  static int retries = 0;
  int dataSize = 0;
  
  HANDLE hSerial = initSerialComm(UART_PORT, UART_BAUD_RATE);
  
  /* Start transmit between host and probe */
  uartSendBytes(hSerial, txBuffer, sizeof(txBuffer));
  
  while(size > 0)  {
    dataSize = tlvCheckDataSize(size);
    
    while(uartGetByte(hSerial) != PROBE_OK)  {
      if(retries++ == 3)  {
        retries = 0;
        printf("Data transmit timeout!\n");
        closeSerialPort(hSerial);
        return;
      }
      tlvWriteDataChunk(dataAddress, destAddress, dataSize, hSerial);
    }

    retries = 0;
    /* Updata data address and size */
    size = size - TLV_DATA_SIZE;
    dataAddress = dataAddress + dataSize;
  }
  
  closeSerialPort(hSerial);
}

/** tlvCheckAcknowledge is function to change the acknowledge reply from probe
  * and decise the next tlv state
  *
  * input     : acknowledge is the response from probe
  *
  * return    : TLV State 
  */
TLV_State tlvCheckAcknowledge(uint8_t acknowledge)  {
  static count = 0;
  
  if(acknowledge == PROBE_OK)  {
    // printf("Probe reply OK!\n");
    return TLV_START;
  }
      
  else if(acknowledge == PROBE_FAULT)  {
    /** Retries 3 times if probe still reply fault acknowledgement it may due to 
      * 1.  Wire connection problem
      * 2.  Power supply problem
      * And many other possible season can cause this problem
      */
      if(count++ == 3)  {
        count = 0; //clear count
        return TLV_ABORT;
      }
      else  {
        return TLV_TRANSMIT_DATA;
      }
  }
  
  else if(acknowledge == PROBE_COMPLETE)
    return TLV_COMPLETE;
  
  else
    printf("Invalid response from PROBE\n");
}

/** tlvHost is state machine for tlv host transmitter
  * 
  */
// void tlvHost(TLVSession *tlvSession)  {
  // TLV *tlv;
  // uint8_t bufferState;
  // uint8_t txBuffer[1024], rxBuffer = 0;
  
  // switch(tlvSession->state)  {
    // case TLV_START :
      // /* Create new TLV packet */
      // tlv = tlvCreateNewPacket(TLV_WRITE);

      // if(tlvSession->pElf->size == 0) {
        // tlvSession->state = TLV_END;
        // break;
      // }
      // else  {
        // /* Get data from elf file */
        // tlvGetDataFromElf(tlv, tlvSession->pElf);
        // /* Pack into TXBUFFER */
        // tlvPackPacketIntoBuffer(txBuffer, tlv);
      // }
      // tlvSession->state = TLV_TRANSMIT_DATA;
      // break;
      
    // case TLV_TRANSMIT_DATA :
      // /* Transmit all data inside txBuffer to probe */
      // printf("Data Starting To Transfer\n");
      // uartSendBytes(tlvSession->hSerial, txBuffer, sizeof(txBuffer));
      // tlvSession->state = TLV_WAIT_REPLY;
      // break;
      
    // case TLV_WAIT_REPLY :
      // /* Waiting reply from probe */
      // rxBuffer = uartGetByte(tlvSession->hSerial);
      // tlvSession->state = tlvCheckAcknowledge(rxBuffer);
      // break;
    
    // case TLV_END  :
      // txBuffer[0] = TLV_TRANSFER_COMPLETE;
      // uartSendBytes(tlvSession->hSerial, txBuffer, sizeof(txBuffer));
      // /* Waiting reply from probe */
      // rxBuffer = uartGetByte(tlvSession->hSerial);
      // tlvSession->state = tlvCheckAcknowledge(rxBuffer);
      // break;
  // }
// }