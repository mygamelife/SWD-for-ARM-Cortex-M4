#include "Flash.h"

static FLASH_EraseInitTypeDef EraseInitStruct;
static FLASH_ErrorTypeDef FLASH_ERROR_CODE = 0;
static uint32_t SectorError = 0;

/**
  * flashMassErase use to perform bank 1, 2 erase or full chip erase both bank
  *
  * Note : if banks value not the expected value as shown below it will select 
  *        the default value FLASH_BANK_2
  *        
  * input     : banks parameter can be one of the following values:
  *               - FLASH_BANK_1: Bank1 to be erased
  *               - FLASH_BANK_2: Bank2 to be erased
  *               - FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * return    : NONE
  */
void flashMassErase(uint32_t banks)  {
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();
  
  /* Fill EraseInit structure */
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_MASSERASE;
  /* VoltageRange value can be modify please refer to datasheet for the voltage range */
  EraseInitStruct.VoltageRange  = FLASH_USER_VOLTAGE_RANGE;
  EraseInitStruct.Banks         = banks;  
  
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /** While error occur during erase process error will be handle here **/
    FLASH_ERROR_CODE = HAL_FLASH_GetError();
    flashErrorHandler();
  }
  
  /** Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) **/
  HAL_FLASH_Lock();
}

/** flashErase is used to erase memory in flash
  *
  * input   : startSector is the starting address to erase
  *           size is the the size of flash memory need to erase
  *
  * output  : NONE
  */

void flashErase(uint32_t flashAddress, int size)  {
  uint32_t firstSector = 0, numOfSectors = 0;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();

  /* Get the 1st sector to erase */
  firstSector = flashGetSector(flashAddress);
  /* Get the number of sector to erase from 1st sector*/
  numOfSectors = flashGetSector(flashAddress + size) - firstSector + 1;

  /* Fill EraseInit structure */
  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
  /* VoltageRange value can be modify please refer to datasheet for the voltage range */
  EraseInitStruct.VoltageRange  = FLASH_USER_VOLTAGE_RANGE;
  EraseInitStruct.Sector        = firstSector;
  EraseInitStruct.NbSectors     = numOfSectors;
  
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /** While error occur during erase process error will be handle here **/
    FLASH_ERROR_CODE = HAL_FLASH_GetError();
    flashErrorHandler();
  }

  /** Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) **/
  HAL_FLASH_Lock();  
}

/** flashWriteByte is a function to write data to flash in byte
  *
  * input   : typeProgram can be one of the following value :
  *            + FLASH_TYPEPROGRAM_BYTE
  *            + FLASH_TYPEPROGRAM_HALFWORD
  *            + FLASH_TYPEPROGRAM_WORD
  *            + FLASH_TYPEPROGRAM_DOUBLEWORD
  *
  *           address is the flash address want to write to 
  *           data is the data need to write to flash
  *
  * return :   NONE
  */
void flashWriteProgram(uint32_t typeProgram, uint32_t address, uint32_t data) {
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();
  
  if(HAL_FLASH_Program(typeProgram, address, data) != HAL_OK)  {
    FLASH_ERROR_CODE = HAL_FLASH_GetError();
    flashErrorHandler();
  }
  
  /* Lock the Flash to disable the flash control register access (recommended
   to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();
}

/** 2 Mbyte dual bank organization can choose between sector 0 - sector 23
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t flashGetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;  
  }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_SECTOR_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_SECTOR_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_SECTOR_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_SECTOR_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_SECTOR_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_SECTOR_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_SECTOR_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_SECTOR_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_SECTOR_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_SECTOR_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_SECTOR_22;  
  }
  else /*((Address >= FLASH_SECTOR_23) && (Address <= ADDR_FLASH_SECTOR_24))*/
  {
    sector = FLASH_SECTOR_23;      
  }
  
  return sector;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * HAL_FLASH_ERROR_NONE         ((uint32_t)0x00000000)    *! No error                      
  * HAL_FLASH_ERROR_RD           ((uint32_t)0x00000001)    *! Read Protection error         
  * HAL_FLASH_ERROR_PGS          ((uint32_t)0x00000002)    *! Programming Sequence error    
  * HAL_FLASH_ERROR_PGP          ((uint32_t)0x00000004)    *! Programming Parallelism error 
  * HAL_FLASH_ERROR_PGA          ((uint32_t)0x00000008)    *! Programming Alignment error   
  * HAL_FLASH_ERROR_WRP          ((uint32_t)0x00000010)    *! Write protection error        
  * HAL_FLASH_ERROR_OPERATION    ((uint32_t)0x00000020)    *! Operation Error               
  *
  * @param  None
  * @retval None
  */
void flashErrorHandler(void)
{
  /* Turn LED4 on */
  turnOnLED4();
  while(1)  {}
}

/** flashCopyFromSramToFlash is a function copy data from SRAM to FLASH
  *
  * input : *src is a source address of SRAM
  *         *dest is the destination address of FLASH
  *         size use to determine how large is the data
  *
  * output :   NONE
  */
void flashCopyFromSramToFlash(uint32_t src, uint32_t dest, int size) {
  int i;
  __IO uint8_t data = 0;
  uint32_t sramAddress = src, flashAddress = dest;
  
  /* Copy data to flash */
  for(i = 0; i < size; i += BYTE_SIZE, sramAddress += BYTE_SIZE, flashAddress += BYTE_SIZE) {
    data = (uint8_t)readMemoryData(sramAddress);
    flashWriteByte(flashAddress, data);
  }
}
