#ifndef SystemConfigure_H
#define SystemConfigure_H

#include "configurePort.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_pwr.h"
#include "stm32f4xx_hal_pwr_ex.h"
#include "stm32f4xx_hal_rcc.h"

void SystemClock_Config(void);
void FlashSystemConfig(void);

#endif // SystemConfigure_H
