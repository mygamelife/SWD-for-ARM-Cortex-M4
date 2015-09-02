#ifndef __MAIN_H
#define __MAIN_H

typedef enum  {
  UART_START,
  UART_WAIT,
  UART_END
} UART_STATE;

#include "swd.h"
#include "uart.h"
#include "configurePort.h"
#include "Register_ReadWrite.h"
#include "CoreDebug.h"
#include "FPB_Unit.h"
#include "DWT_Unit.h"
#include "swdStub.h"
#include "Tlv.h"
#include "SystemConfigure.h"
#include "CException.h"
#endif // main_H
