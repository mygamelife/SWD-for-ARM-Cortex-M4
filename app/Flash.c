#include "Flash.h"

static FLASH_EraseInitTypeDef EraseInitStruct;
static FLASH_ErrorTypeDef FLASH_ERROR_CODE = 0;
static uint32_t SectorError = 0;
static int timeExpired = 1;
static State state = START;

/**
  * Flash_MassErase use to perform bank 1, 2 erase or full chip erase both bank
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
void Flash_MassErase(uint32_t banks)  {
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
    
    #if !defined(TEST)
      Flash_Error_Handler();
    #endif
  }
  
  /** Lock the Flash to disable the flash control register access (recommended
      to protect the FLASH memory against possible unwanted operation) **/
  HAL_FLASH_Lock();
}

/**
  * eraseFlashMemory is used to erase memory in flash can either sector or mass erase
  * stm32f4xx default is using dual-bank memory organization which means user can choose sector 0 - 23
  *
  * input :  typeErase
  *            + FLASH_TYPEERASE_SECTORS      
  *            + FLASH_TYPEERASE_MASSERASE
  * output :   NONE
  */

void Flash_EraseSector(uint32_t startSector, uint32_t endSector)  {
  uint32_t firstSector = 0, numOfSectors = 0;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();

  /* Get the 1st sector to erase */
  firstSector = Flash_GetSector(startSector);
  /* Get the number of sector to erase from 1st sector*/
  numOfSectors = Flash_GetSector(endSector) - firstSector + 1;

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
    Flash_Error_Handler();
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
  *
  *         data is a 32-bit data define by user
  *
  * output :   NONE
  */
void Flash_Write(uint32_t startAddr, uint32_t endAddr, uint32_t typeProgram, uint32_t data) {
  uint32_t address = 0;
  
  address = startAddr;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();
  
  /* Write data into user selected area here */
  while (address < endAddr)
  {
    if (HAL_FLASH_Program(typeProgram, address, data) == HAL_OK)
    {
      address = address + 4;
    }
    else
    {
      /* While error occur during erase process error will be handle here */
      FLASH_ERROR_CODE = HAL_FLASH_GetError();
      Flash_Error_Handler();
      break;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();
  
  #if !defined(TEST)
    Flash_Verify(startAddr, endAddr, data);
  #endif
}

/**
  * verifyWriteData is a function to verify the flashed data is program correctly as expected
  * If data programmed correctly LED3 will turn on
  * Else LED4 will turn on to indicate data is not programmed correctly
  *
  * input : startAddress is the starting address to program it is define by user
  *         dataToVerify is a 32-bit data use to verify the flashed data
  *
  * output :   NONE
  */
void Flash_Verify(uint32_t startAddr, uint32_t endAddr, uint32_t dataToVerify)  {
	__IO uint32_t data32 = 0, MemoryProgramStatus = 0;
	uint32_t address = 0;
	__IO uint32_t tick = 0;
  
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  address = startAddr;

  MemoryProgramStatus = 0x0;
  
  while (address < endAddr)
  {
    data32 = (__IO uint32_t)Flash_Read(address);

    /* Check if they are the same */
    if (data32 != dataToVerify)
    {
      MemoryProgramStatus++;
    }

    address = address + 4;
  }

  /* Check if there is an issue to program data */
  if (MemoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED3 */
    #if !defined(TEST)
      while(timeExpired)	{
        LED3_Blink(&state, &timeExpired);
      }
    #endif
  }
  else
  {
    /* Error detected. Switch on LED4 */
    Flash_Error_Handler();
  }
}

/**
  * Flash_Read is a function to read the data from the corresponding address and return the value
  *
  * input : address is the starting address to program it is define by user
  *
  * output :  data32 is the value store inside the address
  */
uint32_t Flash_Read(uint32_t address)  {
	__IO uint32_t data32 = 0;
  
  data32 = *(__IO uint32_t*)address;
  return data32;
}

/** 2 Mbyte dual bank organization can choose between sector 0 - sector 23
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t Flash_GetSector(uint32_t Address)
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
void Flash_Error_Handler(void)
{
  uint32_t CHECK_ERROR_CODE = 0;

  CHECK_ERROR_CODE = FLASH_ERROR_CODE;

  /* Turn LED4 on */
  turnOnLED4();

  #if !defined(TEST)
    while(1)  {}
  #endif
}

/**
  * Flash_CopyFromSramToFlash is a function copy data from SRAM to FLASH
  *
  * input : *src is a source address of SRAM
  *         *dest is the destination address of FLASH
  *         length use to determine how large is the data
  *
  * output :   NONE
  */
void Flash_CopyFromSramToFlash(uint32_t src, uint32_t dest, int length) {
  int i;
  __IO uint32_t data32 = 0;
  uint32_t FLASH_Addr = 0, SRAM_Addr = 0;

  /* Assign src and dest to a template variable */
  SRAM_Addr	 = src;
  FLASH_Addr = dest;
  
  /* Unlock the Flash to enable the flash control register access */
  HAL_FLASH_Unlock();

  /* Copy data to flash */
  for(i = 0; i < length; i += 4)	{
	  data32 = *(__IO uint32_t *)SRAM_Addr;

	  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_Addr, data32) != HAL_OK)	{
		 Flash_Error_Handler();
		 break;
	  }
    
	  FLASH_Addr = FLASH_Addr + 4;
	  SRAM_Addr	 = SRAM_Addr + 4;
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();

  #if !defined(TEST)
    Flash_VerifyDataFromSramToFlash(src, dest, length);
  #endif
}

/**
  * verifyDataFromRamToFlash is a function to verify the data is correctly copy
  * to flash from SRAM
  *
  * If data programmed correctly LED3 will turn on
  * Else LED4 will turn on to indicate data is not programmed correctly
  *
  * input : *src is the starting address to program it is define by user
  *         *dest is a 32-bit data use to verify the flashed data
  *         length determine how large is the data
  *
  * output :   NONE
  */
void Flash_VerifyDataFromSramToFlash(uint32_t src, uint32_t dest, int length)  {
  int i = 0;
  __IO uint32_t dataFlash = 0, dataSRAM = 0, memoryProgramStatus = 0;
  uint32_t SRAM_Addr = 0, FLASH_Addr = 0, tickstart = 0;

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  memoryProgramStatus = 0x0;

  SRAM_Addr	 = src;
  FLASH_Addr = dest;

  /* Compare data in flash with sram  */
  for(i; i < length; i += 4)	{
    
    /* get data from Flash & SRAM  */
	  dataFlash = *(__IO uint32_t *)FLASH_Addr;
	  dataSRAM 	= *(__IO uint32_t *)SRAM_Addr;

	  if (dataFlash != dataSRAM)	{
		  memoryProgramStatus++;
	  }
    
	  FLASH_Addr = FLASH_Addr + 4;
	  SRAM_Addr  = SRAM_Addr + 4;
  }

  /* Check if there is an issue to program data */
  if (memoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED3 */
    #if !defined(TEST)
      while(timeExpired)	{
        LED3_Blink(&state, &timeExpired);
      }
    #endif
  }
  else
  {
    /* Error detected. Switch on LED4 */
    Flash_Error_Handler();
  }
}

/** Flash_WriteToTarget
  *
  */
void Flash_WriteToTarget(uint32_t startAddr, uint32_t endAddr, uint32_t typeProgram, uint32_t data) {
  
}

/** Flash_WriteToTarget
  *
  */
uint32_t Flash_ReadFromTarget(uint32_t startAddress)  {
  
}
