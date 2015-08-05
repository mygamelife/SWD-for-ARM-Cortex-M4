#include "TLV_Probe.h"

/** tlvGetWordAddress is a function get 32bt address from bytes
  *
  * input     : buffer contain address in bytes
  * 
  * return    : address is a 32bit address
  */
uint32_t tlvGetWordAddress(uint8_t *buffer, int index, int length) {
  int i = 0, shiftValue = 24;
  uint32_t address = 0;
  
  for(i; i < length; i++) {
    address |= buffer[index++] << shiftValue;
    shiftValue = shiftValue - 8;
  }
  
  return address;
}

/** tlvDecodePacket is a function the decode the tlv packet and extract all information outp
  *
  * input   :   buffer is a pointer poiniting to an array that contain tlvPacket
  *
  * return  :   tlv is a TLV type structure address
  */
// TLV *tlvDecodePacket(uint8_t *buffer) {
  // int index = 0, i = 0;
  // static TLV tlv;
    
  // /* Type */
  // tlv.type = buffer[index++];

  // /* Length */
  // tlv.length = buffer[index++];

  // /* Error checking */
  // if(tlv.length == 0) {
    // tlv.errorCode = TLV_EMPTY_BUFFER;
    // return;
  // }
  
  // /* Data */
  // for(index; index < tlv.length + 2; index++)  {
    // tlv.value[i++] = buffer[index];
  // }
  
  // return &tlv;
// }