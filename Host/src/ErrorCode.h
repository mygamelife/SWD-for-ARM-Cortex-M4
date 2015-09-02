#ifndef ErrorCode_H
#define ErrorCode_H

typedef enum{
  ERR_RESPONSE_TIMEOUT,
  ERR_DATA_CORRUPTED,
  ERR_INVALID_INSTRUCTION,
} ErrorCode;

void displayErrorCode(ErrorCode err);

#endif // ErrorCode_H