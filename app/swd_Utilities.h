#ifndef swd_Utilities_H
#define swd_Utilities_H

//SWD Protocol bit sequence
#define DP 0
#define AP 1
#define WRITE 0
#define READ 1
#define STARTBIT 1
#define STOPBIT 0
#define PARKBIT 1

//Acknowledgement response
#define OK      0x1
#define WAIT    0x2
#define FAULT   0x4
#define OK_RESPONSE      1
#define WAIT_RESPONSE    2
#define FAULT_RESPONSE   3
#define NO_RESPONSE      4
#define CLRDAPABOT      0x1

#include <stdint.h>
#include "Register_ReadWrite.h"
int calculateParity_SWDRequest(int Address_bit3,int Address_bit2,int APnDP, int ReadWrite);
int calculateParity_32bitData(uint32_t data);
int getSWD_Request(int Address,int APnDP,int ReadWrite);
int checkAckResponse(int ackValue);
uint32_t checkErrorFlag();

void getSWD_AddressBit(int *Address_bit3,int *Address_bit2,int Address);
void takeActionToAckResponse(int ackResponse);
#endif // swd_Utilities_H
