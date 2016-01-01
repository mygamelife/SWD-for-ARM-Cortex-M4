#include "Stub.h"

/** stubInit is to initialize element in swd stub structure
  *
  * Input   : NONE
  * return  : NONE
  */
void stubInit(void) {
  Stub->instruction       = STUB_CLEAR;
  Stub->status            = STUB_OK;
  Stub->dataSize          = 0;
  Stub->flashAddress      = 0;
  Stub->sramAddress       = 0;
  Stub->banks             = 0;
  Stub->sysClockPrescale  = 0;
  Stub->sysClock          = 0;
  Stub->id                = STUB_ID;
}

/** swdStub is small program routine take instruction from swd probe and response
  *
  * input   : swdInstruction is an instruction send by probe
  * return  : NONE
  */
void stub(int stubInstruction) {
  switch(stubInstruction)  {
    case STUB_COPY :
      stubCopy();
      break;
    
    case STUB_ERASE :
      stubErase();
      break;
  
    case STUB_MASSERASE :
      stubMassErase();
      break;
      
    case STUB_PRESCALE_SYSCLK :
      stubPrescaleSystemClock();
      break;
      
    case STUB_GET_SYSCLK :
      stubGetSystemClock();
      break;
      
    default : break;
  }
}

/** stubCopy is small program routine to copy data in SRAM to Flash
  *
  * input   : NONE
  * return  : NONE
  */
void stubCopy(void) {
  
  /* Change target status to busy to prevent other function to interrupt */
  Stub->status = STUB_BUSY;

  flashCopyFromSramToFlash(Stub->sramAddress, Stub->flashAddress, Stub->dataSize);
  
  /* Clear instruction prevent keep erase */
  Stub->instruction = STUB_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  Stub->status = STUB_OK;
}

/**
  * swdSectorErase is small program routine to erase specific 
  * flash sector address given by user
  *
  * input   : NONE
  * return  : NONE
  */
void stubErase(void)  {
  
  /* Change target status to busy to prevent other function to interrupt */
  Stub->status = STUB_BUSY;
  
  flashErase(Stub->flashAddress, Stub->dataSize);
  
  /* Clear instruction prevent keep erase */
  Stub->instruction = STUB_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  Stub->status = STUB_OK;
}

/** stubMassErase is small program routine to erase specific 
  * bank according to the user selection
  *
  * input   : NONE
  * return  : NONE
  */
void stubMassErase(void)  {

  /* Change target status to busy to prevent other function to interrupt */
  Stub->status = STUB_BUSY;
  
  flashMassErase(Stub->banks);
    
  /* Clear instruction prevent keep erase */
  Stub->instruction = STUB_CLEAR;

  /* Tell probe now target is ready for next instruction */
  Stub->status = STUB_OK;
}

void stubPrescaleSystemClock(void) {
  /* Change target status to busy to prevent other function to interrupt */
  Stub->status = STUB_BUSY;

  /* Configure system clock */
  configSystemClock(Stub->sysClockPrescale);
  
  /* Clear instruction prevent keep erase */
  Stub->instruction = STUB_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  Stub->status = STUB_OK;  
}

void stubGetSystemClock(void) {
  /* Change target status to busy to prevent other function to interrupt */
  Stub->status = STUB_BUSY;

  /* Get system clock frequency */
  Stub->sysClock = getSystemClock();
  
  /* Clear instruction prevent keep erase */
  Stub->instruction = STUB_CLEAR;
  
  /* Tell probe now target is ready for next instruction */
  Stub->status = STUB_OK;
}

/** svcServiceHandler is a svc handler written using C language
  * 
  *     +------+
  *     | xPSR |  svc_args[7]
  *     +------+
  *     |  PC  |  svc_args[6]
  *     +------+
  *     |  LR  |  svc_args[5]
  *     +------+
  *     |  R12 |  svc_args[4]
  *     +------+
  *     |  R3  |  svc_args[3]
  *     +------+
  *     |  R2  |  svc_args[2]
  *     +------+
  *     |  R1  |  svc_args[1]
  *     +------+
  *     |  R0  |  svc_args[0]
  *     +------+  
  *  Cortex-M4 stack frame
  *
  */
void svcServiceHandler(unsigned int *svc_args) {

  while(svc_args[0] == 0) {}

  if(svc_args[0] == SVC_REQUEST_SRAM_ADDRESS) {
	  svc_args[1] = 0x20012800;
  }

  else if(svc_args[0] == SVC_REQUEST_COPY) {
	  flashCopyFromSramToFlash((uint32_t)svc_args[1], (uint32_t)svc_args[2], (int)svc_args[3]);
  }
  
  else if(svc_args[0] == SVC_REQUEST_ERASE) {
	  flashErase((uint32_t)svc_args[1], (int)svc_args[2]);
  }
  
  else if(svc_args[0] == SVC_REQUEST_MASS_ERASE) {
	  flashMassErase((uint32_t)svc_args[1]);
  }
  
  svc_args[0] = 0;
}

/* Use a normal C function, the compiler will make sure that this is going
 * to be called using the standard C ABI which ends in a correct stack
 * frame for our SVC call
 */
void svcServiceCall(void)
{
  #if !defined (TEST)
	svc(SVC_SERVICE_CALL);
  #endif
}

/** Check whether is SVCall is active
  *
  * Input  : session contain a element/handler used by tlv protocol
  */
int IsSvcActive(void) {
  unsigned int svcStatus = 0;

  svcStatus = readMemoryData((uint32_t)&SCB->SHCSR);
  if(svcStatus != 0) {
    return 1;
  }

  else return 0;
}