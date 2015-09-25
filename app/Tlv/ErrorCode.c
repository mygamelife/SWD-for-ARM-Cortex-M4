#include "ErrorCode.h"

void displayErrorMessage(CEXCEPTION_T err) {
	switch(err) {
    case TLV_TIME_OUT :               printf("Err : Host time out\n"); break;
    case TLV_CORRUPTED_DATA :         printf("Err : Host received data corrupted\n"); break;
    case TLV_INVALID_COMMAND :        printf("Err : Host received invalid command\n"); break;
    case TLV_NOT_HALTED :             printf("Err : Halt command not responding\n"); break;
    case TLV_CHECKSUM_ERROR :         printf("Err : Host received checksum error\n"); break;
    case TLV_NOT_STEPPED :            printf("Err : Step command not responding\n"); break;
    case TLV_NOT_STEPOVER :           printf("Err : Step over command cant be completed"); break ;
    case TLV_BKPT_NOTHIT : break;
    case TLV_BKPT_MAXSET : break;
    case TLV_WATCHPOINT_NOTHIT : break;
    case PROBE_TLV_TIME_OUT :         printf("Err : Probe time out\n"); break;
    case PROBE_TLV_CORRUPTED_DATA :   printf("Err : Probe received corrupted data\n"); break;
    case PROBE_TLV_INVALID_COMMAND :  printf("Err : Probe received invalid command\n"); break;
    case PROBE_TLV_CHECKSUM_ERROR :   printf("Err : Probe received checksum error\n"); break;
    
    default :                         printf("Err : Undefine error code\n"); break;
  }
}
