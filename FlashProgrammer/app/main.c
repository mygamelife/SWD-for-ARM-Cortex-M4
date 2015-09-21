#include "main.h"

void svcRequestCall(void);
int isSvcActive(void);

#define SVC_WRITE_DATA 1
#define SVC_CALL 2

extern int isr_vector_table;

int main(void)
{
  __IO uint32_t swdInstruction = 0;

  unsigned int svc_Status1 = 0, svcNum, result2;
  /* Reallocate vector table to RAM */
  //SCB->VTOR = &isr_vector_table;
  
  /* Initialize hardware and configure system clock */
  //FlashSystemConfig();

  //blinkingLED(ONE_SEC);
  /* Initialize target status */
  //writeMemoryData(SWD_TARGET_STATUS, TARGET_OK);

  while(1)
  {
    //swdInstruction = (__IO uint32_t)readMemoryData(SWD_INSTRUCTION);
    //swdStub(swdInstruction);
    if(isSvcActive())
      svcRequestCall();
  }
}

/* Use a normal C function, the compiler will make sure that this is going
 * to be called using the standard C ABI which ends in a correct stack
 * frame for our SVC call
 */
void svcRequestCall(void)
{
	SVC(SVC_REQUEST_CALL);
}

int isSvcActive(void) {
  unsigned int svcStatus = SCB->SHCSR & SCB_SHCSR_SVCALLACT_Msk;
  
  if(svcStatus != 0) {
    SCB->SHCSR = 0x0;
    return 1;
  }
  
  else return 0;
}


/** C_SVC_Handler
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
void C_SVC_Handler(unsigned int *svc_args) {
  //unsigned int svcNum = (char *)svc_args[0];
  unsigned int svcNum = svc_args[0];

  switch(svcNum) {
    case SVC_REQUEST_SRAM_ADDRESS :
    	svc_args[1] = 0x20000000;
    break;
  }
}


