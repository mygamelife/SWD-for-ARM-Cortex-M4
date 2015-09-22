#include "swdStub.h"

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
