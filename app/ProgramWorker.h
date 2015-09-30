#ifndef ProgramWorker_H
#define ProgramWorker_H

#include "Tlv.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
#include "swdStub.h"
#include "ErrorCode.h"
#include "CException.h"

#define ENABLE_SVC          0x80

extern uint32_t tempAddress;

/** swdStub request function
  */
int IsStubBusy(void);
void requestStubErase(uint32_t address, int size);
void requestStubMassErase(uint32_t bankSelect);
void requestStubCopy(uint32_t dataAddress, uint32_t destAddress, int size);

/*############################################### FLASH ###############################################*/
void writeTargetFlash(Tlv_Session *session, uint32_t *dataAddress, uint32_t destAddress, int size);
void eraseTargetFlash(Tlv_Session *session, uint32_t address, int size);
void massEraseTargetFlash(Tlv_Session *session, uint32_t bankSelect);

/*############################################### RAM ###############################################*/
void writeDataToRamInChunk(uint32_t *dataAddress, uint32_t destAddress, int size);
void writeTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t destAddress, int size);
void readTargetMemory(Tlv_Session *session, uint32_t destAddress, int size);

/*############################################### Register ###############################################*/
void writeTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data);
void readTargetRegister(Tlv_Session *session, uint32_t registerAddress);

/*############################################### Reset ###############################################*/
void performSoftResetOnTarget(Tlv_Session *session);
void performHardResetOnTarget(Tlv_Session *session);
void performVectorResetOnTarget(Tlv_Session *session);

void selectTask(Tlv_Session *session, Tlv *tlv);
void probeTaskManager(Tlv_Session *session);

void haltTarget(Tlv_Session *session);
void runTarget(Tlv_Session *session);

void singleStepTarget(Tlv_Session *session);
void multipleStepTarget(Tlv_Session *session, int nInstructions);

void singleStepOver(Tlv_Session *session);

void setBreakpoint(Tlv_Session *session, uint32_t instructionAddress, int matchingMode);
void setWatchpoint(Tlv_Session *session,uint32_t address,Watchpoint_AddressMask addressMask,
                   uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode);
 
void removeInstructionBreakpoint(Tlv_Session *session, uint32_t instructionAddress);
void removeAllInstructionBreakpoint(Tlv_Session *session);

void stopFlashPatchRemapping(Tlv_Session *session,uint32_t address);
void stopAllFlashPatchRemapping(Tlv_Session *session);
                   
void checkBreakpointEvent(Tlv_Session *session);
void checkWatchpointEvent(Tlv_Session *session);   
#endif // ProgramWorker_H
