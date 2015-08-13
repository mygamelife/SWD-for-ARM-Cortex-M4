#include "TLV_Probe.h"

UART_HandleTypeDef UartHandle;

/** <!For internal use only!>
  * tlvReceiveInstructionFromHost is a function to receive instruction sent by host
  *
  * input     : 
  *
  * return    : NONE
  */
void tlvWaitInstructionFromHost(UART_HandleTypeDef *UartHandle) {
  uint8_t rxBuffer = 0;
  int received = 0;
  
  while(!received) {
    if(HAL_UART_Receive(UartHandle, &rxBuffer, 1, 5000) == HAL_OK)
    {
      if(rxBuffer == TLV_START_TRANSMIT)
        received = 1;
    }
  }
}

/** tlvWriteToTargetRam
  *
  * input     : 
  *
  * return    : NONE
  */
void tlvWriteToTargetRam(TLV_Session *tlvSession)  {
	uint8_t txBuffer[1] = {PROBE_OK};
	uint8_t rxBuffer[1024] = {0};
	int received = 0;

  switch(tlvSession->state) {
    case TLV_INITIATE :
    	tlvWaitInstructionFromHost(tlvSession->UartHandle);
    	// Testing
    	if(HAL_UART_Transmit(tlvSession->UartHandle, txBuffer, sizeof(txBuffer), 5000)!= HAL_OK)
    	{
    		errorHandler();
    	}
    	//
    	tlvSession->state = TLV_RECEIVE_PACKET;
      break;
      
    case TLV_RECEIVE_PACKET :
    	while(rxBuffer[0] != TLV_END_TRANSMIT) {
    		if(HAL_UART_Receive(tlvSession->UartHandle, rxBuffer, sizeof(rxBuffer), 5000) == HAL_OK)
    		{
    			//write to ram here

    			/* Reply */
    	    	if(HAL_UART_Transmit(tlvSession->UartHandle, txBuffer, sizeof(txBuffer), 5000)!= HAL_OK)
    	    	{
    	    		errorHandler();
    	    	}
    		}
    	  }
    	tlvSession->state = TLV_END;
      break;

    case TLV_END :
      break;
  }
}

/** tlvGetValue is a function to get tlv value inside the buffer
  *
  * input     : buffer contain length in bytes
  *             index is the start position of the length
  *
  * return    : length value
  */
// void tlvGetValue(uint8_t *buffer, uint8_t *tlvBuffer, int index, int length) {
  // int i = 0;
  
  // for(i; i < length; i++) {
    // tlvBuffer[i] = buffer[i + index];
  // }
// }

/** tlvVerifyType is a function to verify tlv type
  *
  * input     : type is a variable contain tlv type
  *
  * return    : 0   data is invalid
  *             1   data is valid
  */
// int tlvVerifyType(uint8_t type) {
  // if(type == TLV_WRITE)
    // return 1;
  
  // else if(type == TLV_TRANSFER_COMPLETE)
    // return 1;
  
  // else return 0;
// }

/** tlvVerifyLength is a function to verify tlv length
  *
  * input     : length is a variable contain tlv length
  *
  * return    : 0   data is invalid
  *             1   data is valid
  */
// int tlvVerifyLength(int length) {
  // if(length > 0)
    // return 1;
  
  // else
    // return 0;
// }

/** tlvVerifyValue is a function to verify tlv data by adding
  * with the checksum value
  *
  * input     : data is a pointer pointing to TLV structure
  *
  * return    : 0   data is invalid
  *             1   data is valid
  */
// int tlvVerifyValue(TLV *data) {
  // int i = 0, length = data->length - ADDRESS_LENGTH;
  // uint8_t sum = 0;
  
  // for(i; i < length; i++) {
    // sum += data->value[i];
    // printf("data->value[] %x\n", data->value[i]);
  // }
  
  // if(sum == 0)
    // return 1;
  
  // return 0;
// }

/** tlvDecodePacket is a function the decode the tlv packet and extract all information outp
  *
  * input   :   buffer is a pointer poiniting to an array that contain tlvPacket
  *
  * return  :   tlv is a TLV type structure address
  */
// TLV *tlvDecodePacket(uint8_t *buffer) {
  // static TLV tlv;
  // tlv.errorCode = TLV_CLEAR_ERROR;
  
  // tlv.type  = buffer[0];
  // if(!tlvVerifyType(tlv.type)) {
    // tlv.errorCode  =  TLV_INVALID_TYPE;
    // return &tlv;
  // } 
  
  // tlv.length  = buffer[1];
  // if(!tlvVerifyLength(tlv.length)) {
    // tlv.errorCode  =  TLV_INVALID_LENGTH;
    // return &tlv;
  // }
  
  // tlv.sectionAddress  = tlvConvertFromByteToWord(buffer, 2);
  
  // tlvGetValue(buffer, tlv.value, 6, tlv.length - ADDRESS_LENGTH);
  // if(!tlvVerifyValue(&tlv)) {
    // tlv.errorCode  =  TLV_INVALID_VALUE;
    // return &tlv;
  // }
  
  // return &tlv;
// }

/** tlvConvertDataFromByteToWord is a function convert the data from bytes to word
  *
  * input   :   buffer contain all the information in bytes
  *             index is the start position to convert to word
  *
  * return  :   NONE
  */
// uint32_t tlvConvertFromByteToWord(uint8_t *buffer, int index) {
  // int i = 0, shiftValue = 24;
  // uint32_t data32 = 0;
  
  // for(i; i < 4; i++) {
    // data32 |= buffer[index++] << shiftValue;
    // shiftValue = shiftValue - 8;
  // }
  
  // printf("data %x\n", data32);
  
  // return data32;
// }

/** tlvCheckOperation is a function check operation and perform task accordingly
  *
  * input   :   tlv is a pointer pointing to TLV structure
  *
  * return  :   NONE
  */
// void tlvCheckOperation(TLV *tlv) {
  // if(tlv->type == TLV_WRITE)  {
    // /* Write data into RAM */
    
    // memoryAccessWrite(tlv->sectionAddress, tlvConvertFromByteToWord(tlv->value, 0));
  // }
  // else  {
    // /* implement other function here */
  // }
  
// }

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
            // }
          // }
          
          // check operation {
            // if(Type == WRITE) {
              // write_into_sram
              // convertDataToWord {
              // {0xAB,0xCD,0xEF,0x00} = {0xABCDEF00};
            // }
          // }
          
          // reply to host {
            // transmit reply here
          // }
          
        // }
      // }
  //
