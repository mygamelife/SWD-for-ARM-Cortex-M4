#include "TLV_Probe.h"


/** tlvGetWordAddress is a function to get the tlv address from the buffer
  *
  * input     : buffer contain address in bytes
  *             index is the start position of the address
  *
  * return    : address is a 32bit address
  */
uint32_t tlvGetWordAddress(uint8_t *buffer, int index) {
  int i = 0, shiftValue = 24;
  uint32_t address = 0;
  
  for(i; i < ADDRESS_LENGTH; i++) {
    address |= buffer[index++] << shiftValue;
    shiftValue = shiftValue - 8;
  }
  
  return address;
}

/** tlvGetValue is a function to get tlv value inside the buffer
  *
  * input     : buffer contain length in bytes
  *             index is the start position of the length
  *
  * return    : length value
  */
void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length) {
  int i = 0;
  
  for(i; i < length; i++) {
    tlvBuffer[i] = buffer[i + index];
  }
}

/** tlvDecodePacket is a function the decode the tlv packet and extract all information outp
  *
  * input   :   buffer is a pointer poiniting to an array that contain tlvPacket
  *
  * return  :   tlv is a TLV type structure address
  */
TLV *tlvDecodePacket(uint8_t *buffer) {
  static TLV tlv;
    
  tlv.type            = buffer[0];
  tlv.length          = buffer[1];
  tlv.sectionAddress  = tlvGetWordAddress(buffer, 2);
  tlv.chksum          = buffer[tlv.length + 1];
  
  tlvGetValue(buffer, tlv.value, 6, tlv.length - ADDRESS_LENGTH - CHECKSUM_LENGTH);
  
  return &tlv;
}



    //        Pseudocode
    // state diagram {
       // switch(state) {
          // receive {
            // waiting data to arrive
          // }
          
          // decode  {
            // getType;
            // verifyType;
            // errorChecking;
            
            // getLength;
            // verifyLength;
            // errorChecking;
            
            // getData;
            // verifyData;
            // errorChecking;
            
            // no error?
            // convertDataToWord {
              // {0xAB,0xCD,0xEF,0x00} = {0xABCDEF00};
            // }
          // }
          
          // check operation {
            // if(Type == WRITE) {
              // write_into_sram
            // }
          // }
          
          // reply to host {
            // transmit reply here
          // }
          
        // }
      // }
  //