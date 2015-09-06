#ifndef ProgramLoader_H
#define ProgramLoader_H

#include "Tlv.h"
#include "GetHeaders.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "Message.h"

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
// void tlvWriteTargetRam(Tlv_Session *session, User_Session *userSession);
void tlvWriteTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t *destAddress, int *size);

void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size);
void tlvReadTargetRam(Tlv_Session *session, uint32_t *destAddress, int *size);

void commandInterpreter(Tlv_Session *session);

void selectCommand(Tlv_Session *session, User_Session *userSession);
void hostInterpreter(Tlv_Session *session);
#endif // ProgramLoader_H

/*
DataInfo dataInfo(fileName, section) {
  static DataInfo dataInfo;
  ElfData *elfData = openElfFile(fileName);//"test/ELF_File/FlashProgrammer.elf"
  int index = getIndexOfSectionByName(elfData, section);//".text"
  dataInfo->dataAddress = (uint8_t *)getSectionAddress(elfData, index);
  dataInfo->destAddress = getSectionHeaderAddrUsingIndex(elfData, index);
  dataInfo->fileSize = getSectionSize(elfData, index);
  
  return &dataInfo;
}

isrVecotrSection = dataInfo(fileName, ".isr_vector");
roDataSection = dataInfo(fileName, ".rodata");
dataSection = dataInfo(fileName, ".data");
textSection = dataInfo(fileName, ".text");

void tlvWriteTargetRam(Tlv_Session *session, Tlv_DataInfo *dataInfo) {
  session->ONGOING_PROCESS_FLAG = true;

  if(dataInfo->size > TLV_DATA_SIZE) {
      tlvWriteDataChunk(session, dataInfo->dataAddress, dataInfo->destAddress, TLV_DATA_SIZE);
    else
      tlvWriteDataChunk(session, dataInfo->dataAddress, dataInfo->destAddress, dataInfo->size);
    
    dataInfo->dataAddress += TLV_DATA_SIZE;
    dataInfo->destAddress += TLV_DATA_SIZE;
    dataInfo->size -= TLV_DATA_SIZE;    
}

void tlvLoadProgramToRam(Tlv_Session *session, Tlv_DataInfo *dataInfo) {
  switch(state)
    case LOAD_ISR_VECTOR :
      if(isrVecotrSection->size > 0)
        tlvWriteTargetRam(session, isrVecotrSection);
      else state = LOAD_RO_DATA;
      break;
      
    case LOAD_RO_DATA :
      if(roDataSection->size > 0)
        tlvWriteTargetRam(session, roDataSection);
      else state = LOAD_DATA;
      break;
      
    case LOAD_DATA :
      if(roDataSection->size > 0)
        tlvWriteTargetRam(session, roDataSection);
      else state = LOAD_TEXT;
      break;
      
    case LOAD_TEXT :
      if(roDataSection->size > 0)
        tlvWriteTargetRam(session, roDataSection);
      else state = LOAD_ISR_VECTOR;
      break;  
    
}
*/