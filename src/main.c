#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
#include "TLV_Host.h"
#include "GetHeaders.h"

int main(void) {
  TLVSession tlvSession;

  initElfData();
  
  /* Initialize TlvSeesion structure */
  tlvSession.state = TLV_START;
  tlvSession.pElf = elfGetSectionInfoFromFile("C:/Users/susan_000/Desktop/SWD-for-ARM-Cortex-M4/test/ELF_File/FlashProgrammer.elf", ".text");
  tlvSession.hSerial = initSerialComm("COM7", 115200);
  
  printf("Open Serial Port\n");
  
  /* Main Function run here */
  while(tlvSession.state != TLV_COMPLETE) {
    tlvHost(&tlvSession);
  }

  if(tlvSession.state == TLV_COMPLETE)
    printf("TLV Transfer COMPLETE\n");
  
  /* Close serial port, elf file and free and malloc */
  printf("Closing Serial Port\n");  
  closeFileInTxt(dataFromElf->myFile);
  free(pElfSection);
  free(dataFromElf);
  closeSerialPort(tlvSession.hSerial);

  return 0;
}