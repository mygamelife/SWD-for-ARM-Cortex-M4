#ifndef ProgramLoader_H
#define ProgramLoader_H

#include <malloc.h>
#include "Tlv.h"
#include "GetHeaders.h"
#include "Tlv_ErrorCode.h"
#include "CException.h"
#include "Interface.h"
#include "Message.h"

typedef struct {
  int index;
  int size;
  uint8_t *dataAddress;
  uint32_t destAddress;
} FileInfo;

/* Read/Write target register */
void tlvReadTargetRegister(Tlv_Session *session, uint32_t registerAddress);
void tlvWriteTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t *data);

/* Read/Write target RAM */
void tlvWriteDataChunk(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
void tlvWriteTargetRam(Tlv_Session *session, uint8_t **dataAddress, uint32_t *destAddress, int *size);

void tlvReadDataChunk(Tlv_Session *session, uint32_t destAddress, int size);
void tlvReadTargetRam(Tlv_Session *session, uint32_t *destAddress, int *size);

void tlvHaltTarget(Tlv_Session *session);
void tlvRunTarget(Tlv_Session *session);
void tlvMultipleStepTarget(Tlv_Session *session, int nInstructions);

void selectCommand(Tlv_Session *session, User_Session *userSession);
void hostInterpreter(Tlv_Session *session);

FileInfo *extractElfFile(ElfData *elfData, char *section);
// void tlvLoadProgramToRam(Tlv_Session *session, ElfFile elfFile);
void tlvLoadProgramToRam(Tlv_Session *session, char *fileName);

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

isrVectorSection = dataInfo(fileName, ".isr_vector");
roDataSection = dataInfo(fileName, ".rodata");
dataSection = dataInfo(fileName, ".data");
textSection = dataInfo(fileName, ".text");

void tlvWriteTargetRam(Tlv_Session *session, Tlv_DataInfo *dataInfo) {
  session->ongoingProcessFlag = true;

  if(dataInfo->size > TLV_DATA_SIZE) {
      tlvWriteDataChunk(session, dataInfo->dataAddress, dataInfo->destAddress, TLV_DATA_SIZE);
    else
      tlvWriteDataChunk(session, dataInfo->dataAddress, dataInfo->destAddress, dataInfo->size);
    
    dataInfo->dataAddress += TLV_DATA_SIZE;
    dataInfo->destAddress += TLV_DATA_SIZE;
    dataInfo->size -= TLV_DATA_SIZE;    
}
*/