#include "Flash.h"

static FLASH_EraseInitTypeDef EraseInitStruct;
static FLASH_ErrorTypeDef FLASH_ERROR_CODE = 0;

/**
  * eraseFlashMemory is used to erase memory in flash can either sector or mass erase
  * stm32f4xx default is using dual-bank memory organization which means user can choose sector 0 - 23
  *
  * input :  typeErase
  *            + FLASH_TYPEERASE_SECTORS      
  *            + FLASH_TYPEERASE_MASSERASE
  *
  *          banks parameter can be one of the following values:
  *            + FLASH_BANK_1: Bank1 to be erased
  *            + FLASH_BANK_2: Bank2 to be erased
  *            + FLASH_BANK_BOTH: Bank1 and Bank2 to be erased
  *
  *          voltageRange is the device voltage range which defines the erase parallelism.  
  *          This parameter can be one of the following values:
  *            + FLASH_VOLTAGE_RANGE_1: when the device voltage range is 1.8V to 2.1V, 
  *                                     the operation will be done by byte (8-bit) 
  *            + FLASH_VOLTAGE_RANGE_2: when the device voltage range is 2.1V to 2.7V,
  *                                     the operation will be done by half word (16-bit)
  *            + FLASH_VOLTAGE_RANGE_3: when the device voltage range is 2.7V to 3.6V,
  *                                     the operation will be done by word (32-bit)
  *            + FLASH_VOLTAGE_RANGE_4: when the device voltage range is 2.7V to 3.6V + External Vpp, 
  *                                     the operation will be done by double word (64-bit)
  *
  * output :   NONE
  */
void eraseFlashMemory(uint32_t typeErase, uint32_t banks, uint32_t voltageRange) {
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
  EraseInitStruct.VoltageRange = voltageRange;
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
  *
  *         data is a 32-bit data define by user
  *
  * output :   NONE
  */
void writeToFlash(uint32_t typeProgram, uint32_t data) {
  uint32_t address = 0;
  address = FLASH_USER_START_ADDR;
  
  /* Unlock the Flash to enable the flash control register access */ 
  HAL_FLASH_Unlock();
  
  /* Write data into user selected area here */
  while (address < FLASH_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(typeProgram, address, data) == HAL_OK)
    {
      address = address + 4;
    }
    else
    {
      /* While error occur during erase process error will be handle here */
      FLASH_ERROR_CODE = HAL_FLASH_GetError();
      Error_Handler();
      break;
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();

  #ifndef TEST
    verifyWriteData(FLASH_USER_START_ADDR, data);
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
void verifyWriteData(uint32_t startAddress, uint32_t dataToVerify)  {
	__IO uint32_t data32 = 0, MemoryProgramStatus = 0;
	uint32_t address = 0;
  
  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  address = startAddress;

  MemoryProgramStatus = 0x0;
  
  while (address < FLASH_USER_END_ADDR)
  {
    data32 = readFromFlash(address);

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
	  turnOnLED3();
  }
  else
  {
    /* Error detected. Switch on LED4 */
    Error_Handler();
  }
}

/**
  * readFromFlash is a function to read the data from the corresponding address and return the value
  *
  * input : address is the starting address to program it is define by user
  *
  * output :  data32 is the value store inside the address
  */
__IO uint32_t readFromFlash(uint32_t address)  {
	__IO uint32_t data32 = 0;
  
  data32 = *(__IO uint32_t*)address;
  
  return data32;
}

/** 2 Mbyte dual bank organization can choose between sector 0 - sector 23
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
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
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  uint32_t CHECK_ERROR_CODE = 0;

  CHECK_ERROR_CODE = FLASH_ERROR_CODE;

  /* Turn LED4 on */
  turnOnLED4();

  #ifndef TEST
    while(1)  {}
  #endif
}

/**
  * getDataFromRam is a function get data from SRAM
  *
  * input : Address is the sram address
  *
  * output : data32 is data in sram
  */
uint32_t getDataFromRam(uint32_t address)	{
  __IO uint32_t data32 = 0;

  data32 = *(__IO uint32_t *)address;

  return data32;
}

/**
  * copyFromRamToFlash is a function copy data from SRAM to FLASH
  *
  * input : *src is a source address of SRAM
  *			*dest is the destination address of FLASH
  *			length use to determine how large is the data
  *
  * output :   NONE
  */
void copyFromRamToFlash(__IO uint32_t *src, __IO uint32_t *dest, int length) {
  int i;
  __IO uint32_t data32 = 0;
  __IO uint32_t FLASH_Addr = 0, SRAM_Addr = 0;

  /* Assign src and dest to a template variable */
  SRAM_Addr	 = (__IO uint32_t)src;
  FLASH_Addr = (__IO uint32_t)dest;

  /* Unlock the Flash to enable the flash control register access */
  HAL_FLASH_Unlock();

  /* Copy data to flash */
  for(i = 0; i < length; i += 4)	{
	  data32 = *(__IO uint32_t *)SRAM_Addr;

	  if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_Addr, data32) != HAL_OK)	{
		 Error_Handler();
		 break;
	  }
	  FLASH_Addr = FLASH_Addr + 4;
	  SRAM_Addr	 = SRAM_Addr + 4;
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) */
  HAL_FLASH_Lock();

  #ifndef TEST
    verifyDataFromRamToFlash(src, dest, length);
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
  *			length determine how large is the data
  *
  * output :   NONE
  */
void verifyDataFromRamToFlash(__IO uint32_t *src, __IO uint32_t *dest, int length)  {
  __IO uint32_t data32 = 0, memoryProgramStatus = 0, dataSRAM = 0;
  __IO uint32_t SRAM_Addr = 0, FLASH_Addr = 0;
  int i;

  /* Check if the programmed data is OK
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  memoryProgramStatus = 0x0;

  SRAM_Addr	 = (__IO uint32_t)src;
  FLASH_Addr = (__IO uint32_t)dest;

  /* Compare data in flash with sram  */
  for(i = 0; i < length; i += 4)	{
	  data32 	= 	*(__IO uint32_t *)FLASH_Addr;
	  dataSRAM 	= 	*(__IO uint32_t *)SRAM_Addr;

	  if (data32 != dataSRAM)	{
		  memoryProgramStatus++;
	  }
	  FLASH_Addr = FLASH_Addr + 4;
	  SRAM_Addr  = SRAM_Addr + 4;
  }

  /* Check if there is an issue to program data */
  if (memoryProgramStatus == 0)
  {
    /* No error detected. Switch on LED3 */
	  turnOnLED3();
  }
  else
  {
    /* Error detected. Switch on LED4 */
    Error_Handler();
  }
}
