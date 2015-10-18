#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"








 void turnOnLED3();

 void turnOffLED3();

 void turnOnLED4();

 void turnOffLED4();

void configureClock();

void configureTargetResetPin();

void SWDIO_OutputMode();

void SWDIO_InputMode();



void configure_IOPorts();

void configureLED();

void initButton();

uint32_t Button_GetState();

void configureUartPorts();
