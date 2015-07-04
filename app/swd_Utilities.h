#ifndef swd_Utilities_H
#define swd_Utilities_H

#include <stdint.h>
#include "Register_ReadWrite.h"

//SWD Protocol bit sequence
#define DP 0
#define AP 1
#define WRITE 0
#define READ 1
#define STARTBIT 1
#define STOPBIT 0
#define PARKBIT 1

//Acknowledgement response
#define OK_RESPONSE      0x1
#define WAIT_RESPONSE    0x2
#define FAULT_RESPONSE   0x4
#define NO_RESPONSE      -1

int calculateParity_SWDRequest(int Address_bit3,int Address_bit2,int APnDP, int ReadWrite);
int calculateParity_32bitData(uint32_t data);
int getSWD_Request(int Address,int APnDP,int ReadWrite);

void getSWD_AddressBit(int *Address_bit3,int *Address_bit2,int Address);

uint32_t swdCheckErrorFlag();
void swdClearFlags(int ackResponse, int readOrWrite, int address, int APorDP, int parity, uint32_t data);

#define resendSwdDpOperation(readOrWrite, address, ack, parity, data)   swdReadWriteDpWithRetries(readOrWrite, address, ack, parity, data, 1);
#define resendSwdApOperation(readOrWrite, address, ack, parity, data)   swdReadWriteApWithRetries(readOrWrite, address, ack, parity, data, 1);
void swdReadWriteDpWithRetries(int readOrWrite, int address, int *ack, int *parity, uint32_t *data, int counter);
void swdReadWriteApWithRetries(int readOrWrite, int address, int *ack, int *parity, uint32_t *data, int counter);

int retriesSwdOperation(int readOrWrite, int address, int APorDP, int *parity, uint32_t *data, int numOfRetires);
void resendSwdOperation(int readOrWrite, int address, int APorDP, int *parity, uint32_t *data);
#endif // swd_Utilities_H
