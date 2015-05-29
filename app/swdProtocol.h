#ifndef swdProtocol_H
#define swdProtocol_H

#define READ 1
#define WRITE 0
#define DP 0
#define AP 1

void lineReset();
int SWD_Request(int APnDP,int ReadWrite,int Address);
int check_Parity(int ApnDp,int RnW,int AddrBit3,int AddrBit2);
void resetTarget();
int checkAddressbit(int address,int bitNumber);

#endif // swdProtocol_H
