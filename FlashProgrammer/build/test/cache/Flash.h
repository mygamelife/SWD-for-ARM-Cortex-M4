#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash.h"
#include "configurePort.h"
#include "memoryRW.h"
#include "swdStub.h"
















typedef uint32_t FLASH_ErrorTypeDef;

uint32_t flashGetSector(uint32_t Address);



void flashErrorHandler(void);

void flashMassErase(uint32_t banks);

void flashErase(uint32_t flashAddress, int size);

void flashWrite(uint32_t *data, uint32_t address, int size);

void flashCopyFromSramToFlash(uint32_t src, uint32_t dest, int size);



void flashWriteProgram(uint32_t typeProgram, uint32_t address, uint32_t data);
