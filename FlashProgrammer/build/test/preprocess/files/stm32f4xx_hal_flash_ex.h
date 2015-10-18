#include "stm32f4xx_hal_def.h"
typedef struct

{

  uint32_t TypeErase;





  uint32_t Banks;





  uint32_t Sector;





  uint32_t NbSectors;





  uint32_t VoltageRange;





} FLASH_EraseInitTypeDef;









typedef struct

{

  uint32_t OptionType;





  uint32_t WRPState;





  uint32_t WRPSector;





  uint32_t Banks;





  uint32_t RDPLevel;





  uint32_t BORLevel;





  uint8_t USERConfig;



} FLASH_OBProgramInitTypeDef;













typedef struct

{

  uint32_t OptionType;





  uint32_t PCROPState;

  uint32_t Banks;





  uint16_t SectorsBank1;





  uint16_t SectorsBank2;





  uint8_t BootConfig;







} FLASH_AdvOBProgramInitTypeDef;

HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError);

HAL_StatusTypeDef HAL_FLASHEx_Erase_IT(FLASH_EraseInitTypeDef *pEraseInit);

HAL_StatusTypeDef HAL_FLASHEx_OBProgram(FLASH_OBProgramInitTypeDef *pOBInit);

void HAL_FLASHEx_OBGetConfig(FLASH_OBProgramInitTypeDef *pOBInit);







HAL_StatusTypeDef HAL_FLASHEx_AdvOBProgram (FLASH_AdvOBProgramInitTypeDef *pAdvOBInit);

void HAL_FLASHEx_AdvOBGetConfig(FLASH_AdvOBProgramInitTypeDef *pAdvOBInit);

HAL_StatusTypeDef HAL_FLASHEx_OB_SelectPCROP(void);

HAL_StatusTypeDef HAL_FLASHEx_OB_DeSelectPCROP(void);







uint16_t HAL_FLASHEx_OB_GetBank2WRP(void);

void FLASH_Erase_Sector(uint32_t Sector, uint8_t VoltageRange);
