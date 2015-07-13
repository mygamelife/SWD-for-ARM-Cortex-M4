#ifndef Flash_H
#define Flash_H

#include <stdint.h>
#include "configurePort.h"
#include "stm32f4xx_hal_flash.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "stm32f4xx_hal_flash_ramfunc.h"

typedef uint32_t FLASH_ErrorTypeDef;

#define __IO volatile

/** (Erase/Read/Write Area are defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) **/
#define FLASH_USER_START_ADDR       	ADDR_FLASH_SECTOR_21
#define FLASH_USER_END_ADDR         	ADDR_FLASH_SECTOR_23
#define FLASH_USER_VOLTAGE_RANGE    	FLASH_VOLTAGE_RANGE_3

#define sectorErase()               	eraseFlashMemory(FLASH_TYPEERASE_SECTORS, 0, \
														FLASH_USER_VOLTAGE_RANGE);
#define massErase(banks)            	eraseFlashMemory(FLASH_TYPEERASE_MASSERASE, banks, \
														FLASH_USER_VOLTAGE_RANGE);

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 11, 128 Kbytes */
//#define ADDR_FLASH_SECTOR_24     ((uint32_t)0x081FFFFF) /*address within sector 23 use to terminate address not withint sector*/

uint32_t GetSector(uint32_t Address);
uint32_t getDataFromRam(uint32_t address);
__IO uint32_t readFromFlash(uint32_t startAddress);
void Error_Handler(void);
void eraseFlashMemory(uint32_t typeErase, uint32_t banks, uint32_t voltageRange);
void writeToFlash(uint32_t typeProgram, uint32_t data);
void verifyWriteData(uint32_t startAddress, uint32_t dataToVerify);
void copyFromRamToFlash(__IO uint32_t *src, __IO uint32_t *dest, int length);
void verifyDataFromRamToFlash(__IO uint32_t *src, __IO uint32_t *dest, int length);
#endif // Flash_H
