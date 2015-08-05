#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "Serial.h"
// #include "TLV_Host.h"
#include "GetHeaders.h"

int main(void) {
  // TLVSession tlvSession;
  // const char *comPort = "COM7";
  
  /* Create new TLV packet */
  // TLV_TypeDef *tlv = tlvCreateNewPacket(TLV_WRITE);

  initElfData();
  
  dataFromElf->myFile = openFile("C:/Users/susan_000/Desktop/SWD-for-ARM-Cortex-M4/test/ELF_File/FlashProgrammer.elf", "rb+");
  dataFromElf->eh = getElfHeader(dataFromElf);
  dataFromElf->sh = getSectionHeaders(dataFromElf);
  dataFromElf->programElf = getAllSectionInfo(dataFromElf);
  
  printf("dataFromElf->eh->e_shnum %d\n", dataFromElf->eh->e_shnum);
  // printf("dataFromElf->eh->e_type %d\n", dataFromElf->eh->e_type);
  
  uint8_t *pChar = dataFromElf->programElf[4].section;
  printf("pChar %x\n", pChar[0]);
  /* Open elf file */
  // ElfSection *pElf = elfGetSectionInfoFromFile("test/ELF_File/FlashProgrammer.elf", ".text");
  
  /* Put Data into Buffer */
  // int bufferState = tlvPutDataIntoBuffer(tlv, pElf);
  
  //printf("bufferState %d\n", bufferState);
  // initElfData();
  

  // HANDLE hSerial = initSerialComm(comPort, 9600);
  
  /* Initialize TlvSeesion structure */
  // tlvSession.state = TLV_START;
  // tlvSession.pElf = pElfSection;
  // tlvSession.hSerial = hSerial;
  
  //printf("Opening %s\n", comPort);

  // while(1)  {
    // /* Main Function */
    // tlvHost(&tlvSession);
  // }
  
  // tlvHost(&tlvSession);
  //tlvHost(&tlvSession);
  //tlvHost(&tlvSession);
  //printf("rxBuffer %x\n", receiveValue);
  
  // printf("Closing %s\n", comPort);  
//  closeFileInTxt(dataFromElf->myFile);
//  free(pElfSection);
//  free(dataFromElf);
//  closeSerialPort(hSerial);
  
  return 0;
}