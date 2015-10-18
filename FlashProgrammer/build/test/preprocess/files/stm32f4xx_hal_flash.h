#include "stm32f4xx_hal_flash_ramfunc.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_def.h"
typedef enum

{

  FLASH_PROC_NONE = 0,

  FLASH_PROC_SECTERASE,

  FLASH_PROC_MASSERASE,

  FLASH_PROC_PROGRAM

} FLASH_ProcedureTypeDef;









typedef struct

{

  volatile FLASH_ProcedureTypeDef ProcedureOnGoing;



  volatile uint32_t NbSectorsToErase;



  volatile uint8_t VoltageForErase;



  volatile uint32_t Sector;



  volatile uint32_t Bank;



  volatile uint32_t Address;



  HAL_LockTypeDef Lock;



  volatile uint32_t ErrorCode;



}FLASH_ProcessTypeDef;

HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);

HAL_StatusTypeDef HAL_FLASH_Program_IT(uint32_t TypeProgram, uint32_t Address, uint64_t Data);



void HAL_FLASH_IRQHandler(void);



void HAL_FLASH_EndOfOperationCallback(uint32_t ReturnValue);

void HAL_FLASH_OperationErrorCallback(uint32_t ReturnValue);

HAL_StatusTypeDef HAL_FLASH_Unlock(void);

HAL_StatusTypeDef HAL_FLASH_Lock(void);

HAL_StatusTypeDef HAL_FLASH_OB_Unlock(void);

HAL_StatusTypeDef HAL_FLASH_OB_Lock(void);



HAL_StatusTypeDef HAL_FLASH_OB_Launch(void);

uint32_t HAL_FLASH_GetError(void);

HAL_StatusTypeDef FLASH_WaitForLastOperation(uint32_t Timeout);
