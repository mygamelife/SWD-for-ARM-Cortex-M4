#ifndef ProgramWorker_H
#define ProgramWorker_H

#include "Tlv.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
#include "swdStub.h"
#include "ErrorCode.h"
#include "CException.h"

/* swdStub instruction */
void loadEraseSectorInstruction(uint32_t *startSector, uint32_t *endSector);
void loadMassEraseInstruction(uint32_t bankSelect);
void loadCopyFromSRAMToFlashInstruction(uint32_t *dataAddress, uint32_t *destAddress, int size);

void writeTargetRam(Tlv_Session *session, uint32_t *dataAddress, uint32_t destAddress, int size);
void readTargetMemory(Tlv_Session *session, uint32_t destAddress, int size);

void writeTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data);
void readTargetRegister(Tlv_Session *session, uint32_t registerAddress);

void performSoftResetOnTarget(Tlv_Session *session);
void performHardResetOnTarget(Tlv_Session *session);

void selectTask(Tlv_Session *session, Tlv *tlv);
void probeTaskManager(Tlv_Session *session);

void haltTarget(Tlv_Session *session);
void runTarget(Tlv_Session *session);

void singleStepTarget(Tlv_Session *session);
void multipleStepTarget(Tlv_Session *session, int nInstructions);

void setBreakpoint(Tlv_Session *session, uint32_t instructionAddress, int matchingMode);
void setWatchpoint(Tlv_Session *session,uint32_t address,Watchpoint_AddressMask addressMask,
                   uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode);
                   
void checkBreakpointEvent(Tlv_Session *session);
void checkWatchpointEvent(Tlv_Session *session);   
void checkIsSVCActive(Tlv_Session *session);
#endif // ProgramWorker_H
