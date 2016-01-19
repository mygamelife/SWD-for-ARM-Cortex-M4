#ifndef ErrorCode_H
#define ErrorCode_H

#include <stdio.h>
#include "CException.h"

typedef enum
{
  TLV_NULL_SESSION        = 1,
  TLV_TIMEOUT             = 2,
  TLV_INVALID_COMMAND     = 3,
  TLV_CHECKSUM_ERROR      = 4,
  TLV_NOT_HALTED,
  TLV_NOT_RUNNING,
  TLV_NOT_STEPPED,
  TLV_NOT_STEPOVER,
  TLV_NOT_STEPOUT,
  TLV_BKPT_NOTHIT,
  TLV_BKPT_MAXSET,
  TLV_REMAP_MAXSET,
  TLV_ADDRESS_NOT_FOUND,
  TLV_WATCHPOINT_NOTHIT, //12
} Tlv_ErrorCode;

typedef enum
{
  PROBE_NOT_RESPONDING        = -1,
  PROBE_TLV_TIMEOUT           = -2,
  PROBE_TLV_INVALID_COMMAND   = -3,
  PROBE_TLV_CHECKSUM_ERROR    = -4,
  PROBE_STUB_NOT_RESPONDING   = -5,
  PROBE_EVENT_NOT_RESPONDING  = -6,
} Probe_ErrorCode;

typedef enum
{			
	ERR_NO_ERROR = 20,
	ERR_EMPTY_STRING,
	ERR_INCOMPLETE_NUMBER, 
	ERR_NOT_NUMBER_TOKEN,
	ERR_NOT_PREFIX_OPERATOR,
	ERR_STACK_EMPTY,
	ERR_STACK_FULL,
	ERR_IDENTIFIER_NOT_SUPPORT,
	ERR_EXPECTING_NUMBER,
	ERR_UNKNOWN_OPERATOR //29
} Token_ErrorCode;

typedef enum{
	ERR_FILE_NOT_EXIST = 31,
	ERR_BYTE_SELECTION,
  ERR_RANGE_OFFSET, //33
} Elf_ErrorCode;

typedef enum
{			
	ERR_INCOMPLETE_COMMAND = 40,
	ERR_INVALID_USER_COMMAND,
	ERR_INVALID_REGISTER_ADDRESS,
  ERR_EXPECT_NUMBER,
  ERR_EXPECT_REGISTER_ADDRESS,
  ERR_EXPECT_FILE_PATH,
  ERR_EXPECT_MEMORY_SELECTION,
  ERR_INVALID_MEMORY_SELECTION,
  ERR_EXPECT_ERASE_SECTION,
  ERR_INVALID_BANK_SELECTION,
  ERR_OPTION_NOT_FOUND,
  ERR_INVALID_COMMAND,
  ERR_INVALID_MASK,
  ERR_INVALID_DATASIZE,
  ERR_INVALID_ACCESSMODE,
  ERR_EXPECT_WATCHPOINT_MASK,
  ERR_EXPECT_WATCHPOINT_SIZE,
  ERR_EXPECT_WATCHPOINT_MODE,//58
} Interface_ErrorCode;

typedef enum
{
  ERR_INVALID_HANDLER   = 60,
  ERR_SET_COMM_STATE,
  ERR_SET_COMM_TIMEOUTS,
  ERR_NO_COM_PORT,
  ERR_GET_COMM_STATE,
  ERR_INVALID_PASSKEY //65
} Handler_ErrorCode;

typedef enum
{
  ERR_ITM_UNLOCK_FAILED = 70,
} Itm_ErrorCode;

typedef enum
{
  ERR_FP_NOT_EXIST = 80,
} FP_ErrorCode;

void displayErrorMessage(CEXCEPTION_T err);
#endif // ErrorCode_H
