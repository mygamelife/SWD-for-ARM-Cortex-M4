#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "TLV_Protocol.h"
#include "GetHeaders.h"

int main(void) {
  TLVSession tlvSession;

  initElfData();
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = elfGetSectionInfoFromFile("C:/Users/susan_000/Desktop/SWD-for-ARM-Cortex-M4/test/ELF_File/FlashProgrammer.elf", ".text");
  tlvSession.hSerial = initSerialComm("COM7", 9600);
  
  printf("Open Serial Port\n");
  /* Main Function run here */
  tlvHost(&tlvSession);
  tlvHost(&tlvSession);

  //printf("bufferState %d\n", bufferState);
  // while(1)  {
  // }
  // uint8_t rxBuffer[1024];
  // serialGetBytes(tlvSession.hSerial, rxBuffer, sizeof(rxBuffer));
  // printf("Type[0] %x\n", rxBuffer[0]);
  // printf("Length[1] %d\n", rxBuffer[1]);
  // printf("Addr[2] %x\n", rxBuffer[2]);
  // printf("Addr[3] %x\n", rxBuffer[3]);
  // printf("Addr[4] %x\n", rxBuffer[4]);
  // printf("Addr[5] %x\n", rxBuffer[5]);
  
  /* Close serial port, elf file and free and malloc */
  printf("Closing Serial Port\n");  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
  closeSerialPort(tlvSession.hSerial);

  return 0;
}