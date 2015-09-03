#ifndef Tlv_ErrorCode_H
#define Tlv_ErrorCode_H

typedef enum
{
  TLV_TIME_OUT = 1,
  TLV_CORRUPTED_DATA,
  TLV_INVALID_COMMAND,
  TLV_NOT_HALTED,
  TLV_NOT_RUNNING,
  TLV_CHECKSUM_ERROR,
  TLV_NOT_STEPPED,
  TLV_BKPT_NOTHIT,
  TLV_BKPT_MAXSET,
} Tlv_ErrorCode;

#endif // Tlv_ErrorCode_H
