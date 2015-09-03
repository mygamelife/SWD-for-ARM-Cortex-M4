#ifndef Message_H
#define Message_H

typedef enum {
  DO_NOTHING,
  CHANGE_MODE,
  HOST_PROGRAMMER_BUSY,
  HOST_PROGRAMMER_FREE
} Message;

extern Message msg;
#endif // Message_H