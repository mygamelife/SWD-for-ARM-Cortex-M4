#include "Gpio.h"

void gpioUnresetEnableClock(GPIO *port) {
  uint32_t temp = 0;
  
  /* Unreset the CLOCK */
  temp = ~(1 << getGpioIndex(port));
  resetTargetClock(temp);
  
  /* Get the PORTx_CLOCK position */
  temp = 1 << getGpioIndex(port);
  enableTargetClock(temp);
}

void gpioConfigPort(GPIO *port, GPIO_Init *init) {
  uint32_t temp = 0;
  
	gpioUnresetEnableClock(port);
  
  temp = (init->mode << (init->pin * 2));
  setPortMode(port, temp);
  
  temp = ~(1 << init->pin);
  setPortOutputType(port, temp);
  
  temp = (VERY_HIGH_SPEED << (init->pin * 2));
  setPortSpeed(port, temp);
  
  if(init->mode == ALTFUNC_MODE)
    gpioSetAltFunction(port, init->pin, init->af);
}

void gpioWriteOne(GPIO *port, uint16_t pinNum) {
  writePort(port, 1 << pinNum);
}

void gpioWriteZero(GPIO *port, uint16_t pinNum) {
  writePort(port, 0 << pinNum);
}

void pullUpDown(GPIO *port, int pinNum, int pull) {
	port->PUPDR &= ~(3 << (pinNum * 2));
	port->PUPDR |= pull << (pinNum * 2);
}

void gpioSetAltFunction(GPIO *port, int pinNum, int AF) {
  uint32_t temp = 0;
  
  if(pinNum <= 7) {
    temp = AF << (pinNum * 4);
    /* GPIOx_AFRL (for pin 0 to 7) */
    memoryWriteWord((uint32_t)&port->AFRL, temp);
  }
  
  else {
    temp = AF << ((pinNum - 8) * 4);
    /* GPIOx_AFRH (for pin 8 to 15) */
    memoryWriteWord((uint32_t)&port->AFRH, temp);
  }
}