#ifndef swd_Utilities_H
#define swd_Utilities_H

#define DP 0
#define AP 1
#define Write 0
#define Read 1
#define StartBit 1
#define StopBit 0
#define ParkBit 1

#include <stdint.h>
int calculateParity_SWDRequest(int Address_bit3,int Address_bit2,int APnDP, int ReadWrite);
int calculateParity_32bitData(uint32_t data);
int getSWD_Request(int Address,int APnDP,int ReadWrite);

void getSWD_AddressBit(int *Address_bit3,int *Address_bit2,int Address);

#endif // swd_Utilities_H
