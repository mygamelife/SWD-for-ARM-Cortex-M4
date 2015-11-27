#ifndef ProgramWorker_H
#define ProgramWorker_H

#include <stdint.h>
#include "Tlv.h"
#include "CoreDebug.h"
#include "FPBUnit.h"
#include "DWTUnit.h"
#include "SwdStub.h"
#include "CodeStepping.h"
#include "ErrorCode.h"
#include "CException.h"
#include "stm32f4xx_hal.h"
#include "Yield.h"

#define ENABLE_SVC          0x80
#define PROBE_CHANGE_STATE(__SESSION__, __STATE__)      ((__SESSION__)->probeState = __STATE__)

/* Data type mask */
#define BOUNDARY_MASK       0x3
#define WORD_BOUNDARY       0
#define HALFWORD_BOUNDARY   2

#define WORD_SIZE           4
#define WORD_ADDRESS        4
#define HALFWORD_SIZE       2
#define HALFWORD_ADDRESS    2
#define BYTE_SIZE           1
#define BYTE_ADDRESS        1

typedef enum {
  BYTE_TYPE       = 1,
  HALFWORD_TYPE   = 2,
  WORD_TYPE       = 4,
} DataType;

/** swdStub request function
  */
int IsStubBusy(void);
void requestStubErase(uint32_t address, int size);
void requestStubMassErase(uint32_t bankSelect);
void requestStubCopy(uint32_t dataAddress, uint32_t destAddress, int size);

/*############################################### FLASH ###############################################*/
int writeTargetFlash(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);
int eraseTargetFlash(Tlv_Session *session, uint32_t address, int size);
int massEraseTargetFlash(Tlv_Session *session, uint32_t bankSelect);

/*############################################### RAM ###############################################*/
DataType getDataType(uint32_t address, int size);
void writeDataWithCorrectDataType(uint8_t **data, uint32_t *address, int *size);
void writeRamInChunk(uint8_t *dataAddress, uint32_t destAddress, int size);
void writeTargetRam(Tlv_Session *session, uint8_t *dataAddress, uint32_t destAddress, int size);

/*############################################### Register ###############################################*/
void writeTargetRegister(Tlv_Session *session, uint32_t registerAddress, uint32_t data);
void readTargetRegister(Tlv_Session *session, uint32_t registerAddress);
void readAllTargetRegister(Tlv_Session *session);

/*############################################### Reset ###############################################*/
void performSoftResetOnTarget(Tlv_Session *session);
void performHardResetOnTarget(Tlv_Session *session);
void performVectorResetOnTarget(Tlv_Session *session);

void selectTask(Tlv_Session *session, Tlv *tlv);
void probeTaskManager(Tlv_Session *session);

void haltTarget(Tlv_Session *session);
void runTarget(Tlv_Session *session);

void performSingleStepInto(Tlv_Session *session);
void performMultipleStepInto(Tlv_Session *session, int nInstructions);
void performStepOver(Tlv_Session *session);
void performStepOut(Tlv_Session *session);

void setBreakpoint(Tlv_Session *session, uint32_t instructionAddress);
void setWatchpoint(Tlv_Session *session,uint32_t address,Watchpoint_AddressMask addressMask,
                   uint32_t matchedData,Watchpoint_DataSize dataSize,Watchpoint_AccessMode accessMode);
 
void removeHardwareBreakpoint(Tlv_Session *session, uint32_t instructionAddress);
void removeAllHardwareBreakpoint(Tlv_Session *session);

void stopFlashPatchRemapping(Tlv_Session *session,uint32_t address);
void stopAllFlashPatchRemapping(Tlv_Session *session);
                   
void breakpointEventHandler(Tlv_Session *session);
void watchpointEventHandler(Tlv_Session *session);

void eventOccured(Tlv_Session *session, EventType event);
void checkDebugEvent(Tlv_Session *session, EventType event);

/* ### Testing Tlv Protocol ## */
void loopBack(Tlv_Session *session, Tlv *packet);

void comPortVerification(Tlv_Session *session);

#endif // ProgramWorker_H
