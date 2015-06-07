#ifndef swdProtocol_H
#define swdProtocol_H

#define READ 1
#define WRITE 0
#define DP 0
#define AP 1

typedef enum
{
	STICKYORUN ,
	WDATAERR,
	STICKYERR,
	STICKYCMP
} ErrorFlag ;

typedef enum
{
	OK = 1,
	WAIT = 2,
	FAULT = 4,
	WAITED_TOOLONG = 5
} ACK_Response;

void simpleDelay();

void lineReset();
void resetTarget();
void initialisation();
void sendSWDRequest(int SWD_RequestData);

int SWD_Request(int APnDP,int ReadWrite,int Address);
int check_Parity(int ApnDp,int RnW,int AddrBit3,int AddrBit2);
int checkAddressbit(int address,int bitNumber);
int checkACK_RWData(long *data, int SWD_RequestData, int ReadWrite);

int AP_Select(int APnDP,int BankNo,int APSEL);
int ABORT_CLEAR_ERRFLAG(int DAPabort,ErrorFlag errflag);

#endif // swdProtocol_H
