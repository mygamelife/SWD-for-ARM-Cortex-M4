#ifndef Misc_Utilities_H
#define Misc_Utilities_H

#include <stdint.h>

#define Bit_10	10
#define Bit_9	9
#define Bit_8	8
#define Bit_7	7
#define Bit_6	6
#define Bit_5	5
#define Bit_4	4
#define Bit_3	3
#define Bit_2	2
#define Bit_1	1


#define FALSE 0
#define TRUE 1

typedef enum
{
	ERR_NOERROR = 1,
	ERR_INVALID_PARITY_RECEIVED,
	ERR_BITS_SPECIFIED_NOT_SET,
	ERR_DATARW_NOT_MATCH,
	ERR_CORECONTROL_FAILED,
	ERR_COREREGRW_FAILED,
	ERR_FPB_NOTENABLED,
	ERR_FPB_NOTDISABLED,
	ERR_INVALID_COMPARATOR,
	ERR_INVALID_DWTREGISTER
}ErrorCode;


int check_BitSetWithMask(uint32_t data,uint32_t dataMask);
int check_dataWrote_dataRead(uint32_t dataWrote,uint32_t dataRead);


#endif // Misc_Utilities_H
