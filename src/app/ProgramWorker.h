#ifndef ProgramWorker_H
#define ProgramWorker_H

#include "Tlv.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
#include "SwdStub.h"
#include "CodeStepping.h"
#include "ErrorCode.h"
#include "CException.h"
#include "stm32f4xx_hal.h"

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
/* ###### read ###### */
void readTargetMemory(Tlv_Session *session, uint32_t destAddress, int size);
void readTargetInWord(Tlv_Session *session, uint32_t destAddress);
void readTargetInHalfword(Tlv_Session *session, uint32_t destAddress);
/* ###### write ###### */
void writeTargetInWord(Tlv_Session *session, uint32_t address, uint32_t data);
void writeTargetInHalfword(Tlv_Session *session, uint32_t address, uint16_t data);
void writeTargetInWord(Tlv_Session *session, uint32_t address, uint32_t data);
void writeTargetInHalfword(Tlv_Session *session, uint32_t address, uint16_t data);
void writeTargetInByte(Tlv_Session *session, uint32_t address, uint8_t data);

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
void removeAllBreakpoint(Tlv_Session *session);
void removeAllHardwareBreakpoint(Tlv_Session *session);
void removeAllSoftwareBreakpoint(Tlv_Session *session);

void stopFlashPatchRemapping(Tlv_Session *session,uint32_t address);
void stopAllFlashPatchRemapping(Tlv_Session *session);
                   
void breakpointEventHandler(Tlv_Session *session);
void watchpointEventHandler(Tlv_Session *session);


/* ### Testing Tlv Protocol ## */
void loopBack(Tlv_Session *session, Tlv *packet);

typedef enum {
  SET_CSW_VALUE = 1,
  WRITE_ADDRESS,
  READ_VALUE,
} Test_State;
void testing(Test_State *state);
#endif // ProgramWorker_H
