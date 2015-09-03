#ifndef swd_Utilities_H
#define swd_Utilities_H

#include <stdint.h>
#include "Misc_Utilities.h"
#include "Register_ReadWrite.h"


//SWD Protocol bit sequence
#define DP 0
#define AP 1
#define WRITE 0
#define READ 1
#define STARTBIT 1
#define STOPBIT 0
#define PARKBIT 1

typedef enum {
	OK_RESPONSE = 0x01,
	WAIT_RESPONSE = 0x02,
	FAULT_RESPONSE = 0x04
} Swd_Response;
//Acknowledgment response


int calculateParity_SWDRequest(int Address_bit3,int Address_bit2,int APnDP, int ReadWrite);
int calculateParity_32bitData(uint32_t data);
int getSWD_Request(int Address,int APnDP,int ReadWrite);

int compare_ParityWithData(uint32_t data,int Parity);

void getSWD_AddressBit(int *Address_bit3,int *Address_bit2,int Address);

uint32_t swdCheckErrorFlag(void);
void swdClearErrorFlag(uint32_t errorFlag);
void swdErrorHandler(int error, int readOrWrite, int pointType, int address, uint32_t *data);
int swdGetAckResponse(int ack);

// #define resendSwdDpOperation(readOrWrite, address, ack, parity, data)   swdReadWriteDpWithRetries(readOrWrite, address, ack, parity, data, 1);
// #define resendSwdApOperation(readOrWrite, address, ack, parity, data)   swdReadWriteApWithRetries(readOrWrite, address, ack, parity, data, 1);
int swdReadDpWithRetries(int address, uint32_t *data, int counter);
int swdWriteDpWithRetries(int address, uint32_t data, int counter);
int swdReadApWithRetries(int address, uint32_t *data, int counter);
int swdWriteApWithRetries(int address, uint32_t data, int counter);

int swdRetriesOperation(int readOrWrite, int pointType, int address, uint32_t *data, int numOfRetires);
void swdResendOperation(int readOrWrite, int pointType, int address, uint32_t *data);
#endif // swd_Utilities_H
