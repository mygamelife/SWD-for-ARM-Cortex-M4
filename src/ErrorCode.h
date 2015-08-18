#ifndef ErrorCode_H
#define ErrorCode_H

typedef enum{
  ERR_RESPONSE_TIMEOUT,
  ERR_CORRUPTED_DATA,
} ErrorCode;

void displayErrorCode(ErrorCode err);

#endif // ErrorCode_H