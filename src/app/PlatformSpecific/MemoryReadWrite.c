#include "MemoryReadWrite.h"

int cswDataSize = -1;

/* Use for mocking purpose in test_TLV_Probe */
uint32_t memoryReadAndReturnWord(uint32_t address) {
  uint32_t dataRead = 0;
  
  memoryReadWord(address, &dataRead);
  return dataRead;
}

uint8_t memoryReadAndReturnByte(uint32_t address) {
  uint32_t dataRead = 0;
  memoryReadByte(address, &dataRead);

  return (uint8_t)dataRead;
}

int memoryReadByte(uint32_t address, uint32_t *dataRead) {
  int parity = 0 , status = 0; uint32_t data;

  if(cswDataSize != CSW_BYTE_SIZE) // used to prevent setting same size again and again
  {
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_BYTE_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
    cswDataSize = CSW_BYTE_SIZE;
  }
  swdWriteAP(TAR_REG, address);
  swdReadAP(DRW_REG, &data);
  
  /* 0x00 */
  if     ((address & 0x3) == 0x0) *dataRead = ((data & 0xFF       ) >> 0  );
  /* 0x01 */
  else if((address & 0x3) == 0x1) *dataRead = ((data & 0xFF00     ) >> 8  );
  /* 0x10 */
  else if((address & 0x3) == 0x2) *dataRead = ((data & 0xFF0000   ) >> 16 );
  /* 0x11 */
  else if((address & 0x3) == 0x3) *dataRead = ((data & 0xFF000000 ) >> 24 );
  
  status = compareParityWithData(*dataRead,parity);
	
	return status;
}

int memoryReadWord(uint32_t address, uint32_t *dataRead)
{
	int parity = 0 , status = 0;
	
	if(cswDataSize != CSW_WORD_SIZE) // used to prevent setting same size again and again
  {
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_WORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
    cswDataSize = CSW_WORD_SIZE;
  }

	swdWriteAP(TAR_REG, address);
	swdReadAP(DRW_REG, dataRead);
	
	status = compareParityWithData(*dataRead,parity);
	
	return status;
}

int memoryReadHalfword(uint32_t address,uint32_t *dataRead)
{
  int parity = 0 , status = 0;
	uint32_t data;
  
  if(cswDataSize != CSW_HALFWORD_SIZE) // used to prevent setting same size again and again
  {
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE | CSW_ENABLE_ADDR_INC_PACKED));
    cswDataSize = CSW_HALFWORD_SIZE;
  }

  swdWriteAP(TAR_REG, address);
  swdReadAP(DRW_REG, &data);

  /* 0x00 */
  if     ((address & 0x3) == 0x0) *dataRead = ((data & 0xFFFF     ) >> 0  );
  /* 0x01 */
  else if((address & 0x3) == 0x1) *dataRead = ((data & 0xFFFF00   ) >> 8  );
  /* 0x10 */
  else if((address & 0x3) == 0x2) *dataRead = ((data & 0xFFFF0000 ) >> 16 );
  /* 0x11 */
  else if((address & 0x3) == 0x3) {
    *dataRead = (((data & 0xFF000000  ) >> 24 ) | ((data & 0x000000FF ) << 8 ));
  }
  
  status = compareParityWithData(*dataRead,parity);

  return status;
}

SwdError memoryWriteByte(uint32_t address, uint8_t writeData)
{
  int error = 0;
  uint32_t alignedData = 0;
  
  if(cswDataSize != CSW_BYTE_SIZE) // used to prevent setting same size again and again
  {  
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
    cswDataSize = CSW_BYTE_SIZE;
  }
  alignedData = memoryWriteDataAlignment(address,writeData);
  
  error = swdWriteAP(TAR_REG, address);
  if(error != SWD_NO_ERROR) return error;
  
  error = swdWriteAP(DRW_REG, alignedData);
  return error;
}

SwdError memoryWriteHalfword(uint32_t address, uint16_t writeData)
{
  int error = 0;
  uint32_t alignedData = 0;
  
  if(cswDataSize != CSW_HALFWORD_SIZE) // used to prevent setting same size again and again
  {  
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
    cswDataSize = CSW_HALFWORD_SIZE;
  }
  
  alignedData = memoryWriteDataAlignment(address,writeData);
  
  error = swdWriteAP(TAR_REG, address);
  if(error != SWD_NO_ERROR) return error;
  
  error = swdWriteAP(DRW_REG, alignedData);
  return error;
}

SwdError memoryWriteWord(uint32_t address, uint32_t writeData)
{
  int error = 0;
  
  if(cswDataSize != CSW_WORD_SIZE) // used to prevent setting same size again and again
  {  
    swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_WORD_SIZE));
    cswDataSize = CSW_WORD_SIZE;
  }
 
  error = swdWriteAP(TAR_REG, address);
  if(error != SWD_NO_ERROR) return error;
  
  error = swdWriteAP(DRW_REG, writeData);
  return error;
}

int memoryWriteData(uint32_t address, uint8_t *data, int size) {
  int i = 0, tsize = address + size;
  
  if((address & 0x3) == 0) {
    memoryWriteWord(address, *((uint32_t *)data));
    data += 4;
  }
  
  printf("data %x\n", *data);
  
  printf("tsize %x\n", tsize);
  printf("address half %d\n", (0x20000002 & 0x3));
  printf("address word %d\n", (0x20000004 & 0x3));
  printf("address half %d\n", (0x20000006 & 0x3));
  printf("address word %d\n", (0x20000008 & 0x3));
}

/**
 * Perform data alignment when performing byte or halfword memory write access
 *
 *  Input : address is the address where the data is going to be written into
 *          writeData is the data going to be written into the address
 *
 *  Output : return 32bits of aligned data
 */
uint32_t memoryWriteDataAlignment(uint32_t address,uint16_t writeData)
{ 
  if(address % 4 == 0x1)
    return writeData << 8 ;
  if(address % 4 == 0x2)
    return writeData << 16 ;
  if(address % 4 == 0x3)
    return writeData << 24 ;
  
  return writeData ;
}
