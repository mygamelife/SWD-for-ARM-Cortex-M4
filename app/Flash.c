#include "Flash.h"

#define __IO volatile

/** (Erase/Read/Write Area are defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) **/
#define FLASH_USER_START_ADDR     ADDR_FLASH_SECTOR_13
#define FLASH_USER_END_ADDR       ADDR_FLASH_SECTOR_14

static FLASH_EraseInitTypeDef EraseInitStruct;
static void Error_Handler(void);
static uint32_t GetSector(uint32_t Address);
static FLASH_ErrorTypeDef FLASH_ERROR_CODE = 0;

/**
  * eraseFlashMemory is used to erase memory in flash can either sector or mass erase
  * stm32f4xx default is using dual-bank memory organization which means user can choose sector 0 - 23
  *
  * input :  typeErase
  *            + FLASH_TYPEERASE_SECTORS      
  *            + FLASH_TYPEERASE_MASSERASE
  *          banks parameter can be one of the following values:
  *            + FLASH_BANK_1: Bank1 to be erased
  *            + FLASH_BANK_2: Bank2 to be erased
  *            + FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  * output :   NONE
  */
void eraseFlashMemory(uint32_t typeErase, uint32_t banks) {
  FLASH_ErrorTypeDef CHECK_SECTOR_ERROR = 0;
  uint32_t firstSector = 0, numOfSectors = 0, sectorError = 0;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();

  /* Get the 1st sector to erase */
  firstSector = GetSector(FLASH_USER_START_ADDR);
  /* Get the number of sector to erase from 1st sector*/
  numOfSectors = GetSector(FLASH_USER_END_ADDR) - firstSector + 1;

  /* Fill EraseInit structure */
  EraseInitStruct.TypeErase = typeErase;
  /* VoltageRange value can be modify please refer to datasheet for the voltage range */
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Banks = banks;
  EraseInitStruct.Sector = firstSector;
  EraseInitStruct.NbSectors = numOfSectors;
  
  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &sectorError) != HAL_OK)
  { 
    /* If CHECK_SECTOR_ERROR is 0 means selected sector has not been erased correctly*/
    CHECK_SECTOR_ERROR = sectorError;
    /** While error occur during erase process error will be handle here **/
    FLASH_ERROR_CODE = HAL_FLASH_GetError();
    Error_Handler();
  }

  /** Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) **/
  HAL_FLASH_Lock();
}

/**
  * writeToFlash is a function write data is user define address
  * !* Recommend erase flash memory first before write *!
  *
  * input : typeProgram 
  *            + FLASH_TYPEPROGRAM_BYTE
  *            + FLASH_TYPEPROGRAM_HALFWORD
  *            + FLASH_TYPEPROGRAM_WORD
  *            + FLASH_TYPEPROGRAM_DOUBLEWORD
  *         typeErase
  *            + FLASH_TYPEERASE_SECTORS      
  *            + FLASH_TYPEERASE_MASSERASE
  *         data is a 32-bit data define by user
  *
  * output :   NONE
  */
void writeToFlash(uint32_t typeProgram, uint32_t data) {
  uint32_t Address = 0;
  Address = FLASH_USER_START_ADDR;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();
  
  /* Write data into user selected area here */
  while (Address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(typeProgram, Address, data) == HAL_OK)
    {
      Address = Address + 4;
    }
    else
    {
      /* While error occur during erase process error will be handle here */
      FLASH_ERROR_CODE = HAL_FLASH_GetError();
      Error_Handler();
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();
  
  verifyWriteData(FLASH_USER_START_ADDR, data);
}

/**
  * verifyWriteData is a function to verify the data is correctly programmed into flash
  * If data programmed correctly LED3 will turn on
  * Else LED4 will turn on to indicate data is not programmed correctly
  *
  * input : startAddress is the starting address to program it is define by user
  *         flashedData is a 32-bit data define by user
  *
  * output :   NONE
  */
void verifyWriteData(uint32_t startAddress, uint32_t flashedData)  {
	__IO uint32_t data32 = 0, MemoryProgramStatus = 0;
	uint32_t address = 0;
  
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  address = startAddress;
  MemoryProgramStatus = 0x0;

  while (address < FLASH_USER_END_ADDR)
  {
    data32 = *(__IO uint32_t*)address;

    if (data32 != flashedData)
    {
      MemoryProgramStatus++;
    }

    address = address + 4;
  }

  /* Check if there is an issue to program data */
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED3 */
    BSP_LED_On(LED3);
  }
  else
  {
    /* Error detected. Switch on LED4 */
    Error_Handler();
  }  
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
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
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_SECTOR_23;  
  }

  return sector;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED4 on */
  BSP_LED_On(LED4);
  while(1)
  {
  }
}


