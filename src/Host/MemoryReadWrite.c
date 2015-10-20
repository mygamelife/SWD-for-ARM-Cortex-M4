#include "MemoryReadWrite.h"

#ifdef TEST
/* Global Tlv Session */
Tlv_Session *_session = NULL; 
/* Global Error code */
CEXCEPTION_T err = 0;
#endif

// int cswDataSize = -1;
 
// /***************************************************************
 // **                   SWD-DP Register                         **
 // ***************************************************************/

// /******************************************************************************************************
 // |  Start bit   |  APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 // ------------------------------------------------------------------------------------------------------
 // |      1       |    0      |    0     |     0      |     0     |     1       |    0      |     1     |
 // ******************************************************************************************************/

// /**Register Address
 // *  Address        READ          Write
 // *  --------------------------------------
 // *  0x00          IDCODE        ABORT
 // *  0x04          CTRL/STAT     CTRL/STAT
 // *  0x08          RESEND        SELECT
 // *  0x0C          RDBUFF_REG    N/A
 // */

// /**
 // *	Action          Value
 // *  -------------------------------------------------------------------
 // *  START         Always  1
 // *  AP/DP1        DP  = 0,   AP  = 1
 // *  RW            W   = 0,   R   = 1
 // *  Addr[2:3]     Refer Register Address
 // *  Parity        APnDP + RW + Addr2 + Addr3 Even 1's = 0, Odd 1's = 1
 // *  STOP          Always  1
 // *  PARK          Always  1
 // */
 
// SwdError swdRegisterWrite(int address, int pointType, uint32_t data)
// {
	// int swdRequest, parity, ack;

	// swdRequest = getSWD_Request(address, pointType, WRITE);
	// parity = calculateParity_32bitData(data); //calculate parity before initiating transfer

	// send8bit(swdRequest);

	// turnAroundRead();
	// setSWDIOInputMode();

	// read3bit(&ack);

	// turnAroundWrite();
	// setSWDIOOutputMode();

	// send32bit(data);
	// sendBit(parity);

	// extraIdleClock(8);
  
  // return swdGetAckResponse(ack);
// }

// SwdError swdRegisterRead(int address, int pointType, uint32_t *data)
// {
	// int swdRequest, ack, parity;

	// swdRequest = getSWD_Request(address, pointType, READ);
	// send8bit(swdRequest);

	// turnAroundRead();
	// setSWDIOInputMode();

	// read3bit(&ack);
	// read32bit(data);

	// parity = readBit();

	// turnAroundWrite();
	// setSWDIOOutputMode();

	// extraIdleClock(8);
  
  // return swdGetAckResponse(ack);
// }

// SwdError swdReadAP(int address, uint32_t *data)
// {
  // int error;
	// uint32_t discardPreviousRead = 0;
  // error = swdRegisterRead(address, AP, &discardPreviousRead);
  // error = swdRegisterRead(address, AP, data);

	// return error;
// }

// /*  readAHB_IDR is a function to access AHB-AP (based on the implementation of MEM-AP) register and select BankF read the register IDR
 // *
 // *  input   : data_IDR is the variable pass-in by user to store the IDR
 // *  return  : NONE
 // */
// SwdError readAhbIDR(uint32_t *data_IDR)	{
  // int error = 0;

  // /**Power up/wake up debug system before using */
  // swdWriteDP(CTRLSTAT_REG, POWERUP_SYSTEM);

  // error = swdWriteDP(SELECT_REG, SELECT_BANKF);
  // if(error != NO_ERROR) return error;

  // error = swdReadAP(IDR_REG, data_IDR);
  
  // return error;
// }

// /** swdSelectMemorySize is a function to set the memory access size bit in CSW register
  // *
  // * input   : memorySize can be one of the following value
  // *             + CSW_BYTE_SIZE
  // *             + CSW_HALFWORD_SIZE
  // *             + CSW_WORD_SIZE
  // *             + CSW_DISABLE_ADDR_INC
  // *             + CSW_ENABLE_ADDR_INC_SINGLE
  // *             + CSW_ENABLE_ADDR_INC_PACKED
  // *
  // * return  : NONE
  // */
// SwdError swdSelectMemorySize(uint32_t memorySize)  {
  // int error = 0;
  
  // /* Select Bank register 0 */
  // error = swdWriteDP(SELECT_REG, SELECT_BANK0);
  // if(error != NO_ERROR) return error;
  
  // /* Write cswBitSet into CSW register */
  // error = swdWriteAP(CSW_REG, memorySize);
  
  // return error;
// }

// /* Use for mocking purpose in test_TLV_Probe */
// uint32_t memoryReadAndReturnWord(uint32_t address) {
  // uint32_t dataRead = 0;
  
  // memoryReadWord(address, &dataRead);
  // return dataRead;
// }

// /**
 // * Perform data alignment when performing byte or halfword memory write access
 // *
 // *  Input : address is the address where the data is going to be written into
 // *          writeData is the data going to be written into the address
 // *
 // *  Output : return 32bits of aligned data
 // */
// uint32_t memoryWriteDataAlignment(uint32_t address,uint16_t writeData)
// { 
  // if(address % 4 == 0x1)
    // return writeData << 8 ;
  // if(address % 4 == 0x2)
    // return writeData << 16 ;
  // if(address % 4 == 0x3)
    // return writeData << 24 ;
  
  // return writeData ;
// }

// /**
  // ==============================================================================
            // ################ Used To Download Into ARM ################
  // ==============================================================================  
  // */
// #if !defined (TEST)
// int memoryReadWord(uint32_t address, uint32_t *dataRead)
// {
	// int parity = 0 , status = 0;
	
	// if(cswDataSize != CSW_WORD_SIZE) // used to prevent setting same size again and again
	  // {
	    // swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_WORD_SIZE));
	    // cswDataSize = CSW_WORD_SIZE;
	  // }

	// swdWriteAP(TAR_REG, address);
	// swdReadAP(DRW_REG, dataRead);
	
	// status = compare_ParityWithData(*dataRead,parity);
	
	// return status;
// }

// int memoryReadHalfword(uint32_t address,uint32_t *dataRead)
// {
  // int parity = 0 , status = 0;
	
	// if(cswDataSize != CSW_HALFWORD_SIZE) // used to prevent setting same size again and again
	  // {
	    // swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
	    // cswDataSize = CSW_HALFWORD_SIZE;
	  // }

	// swdWriteAP(TAR_REG, address);
	// swdReadAP(DRW_REG, dataRead);
	
	// status = compare_ParityWithData(*dataRead,parity);
	
	// return status;
// }

// SwdError memoryWriteByte(uint32_t address, uint8_t writeData)
// {
  // int error = 0;
  // uint32_t alignedData = 0;
  
  // if(cswDataSize != CSW_BYTE_SIZE) // used to prevent setting same size again and again
  // {  
    // swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_BYTE_SIZE));
    // cswDataSize = CSW_BYTE_SIZE;
  // }
  // alignedData = memoryWriteDataAlignment(address,writeData);
  
  // error = swdWriteAP(TAR_REG, address);
  // if(error != NO_ERROR) return error;
  
  // error = swdWriteAP(DRW_REG, alignedData);
  // return error;
// }

// SwdError memoryWriteHalfword(uint32_t address, uint16_t writeData)
// {
  // int error = 0;
  // uint32_t alignedData = 0;
  
  // if(cswDataSize != CSW_HALFWORD_SIZE) // used to prevent setting same size again and again
  // {  
    // swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_HALFWORD_SIZE));
    // cswDataSize = CSW_HALFWORD_SIZE;
  // }
  
  // alignedData = memoryWriteDataAlignment(address,writeData);
  
  // error = swdWriteAP(TAR_REG, address);
  // if(error != NO_ERROR) return error;
  
  // error = swdWriteAP(DRW_REG, alignedData);
  // return error;
// }

// SwdError memoryWriteWord(uint32_t address, uint32_t writeData)
// {
  // int error = 0;
  
  // if(cswDataSize != CSW_WORD_SIZE) // used to prevent setting same size again and again
  // {  
    // swdSelectMemorySize((CSW_DEFAULT_MASK | CSW_WORD_SIZE));
    // cswDataSize = CSW_WORD_SIZE;
  // }
 
  // error = swdWriteAP(TAR_REG, address);
  // if(error != NO_ERROR) return error;
  
  // error = swdWriteAP(DRW_REG, writeData);
  // return error;
// }

/**
  ==============================================================================
            ################ Used To Testing TDD Purpose ################
  ==============================================================================  
  */
#else
int memoryReadWord(uint32_t address, uint32_t *dataRead) {
  int size = 4; uint8_t *data = NULL;
  
  if(_session == NULL) _session = tlvCreateSession();

  Try {
    /* Waiting reply from probe */
    while((data = tlvReadTargetMemory(_session, &address, &size)) == NULL) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  *dataRead = getDataInWord(&data[4]);
  
  return 1;
}

int memoryWriteWord(uint32_t address, uint32_t writeData) {
  int status = 0;
  
  if(_session == NULL) _session = tlvCreateSession();

  Try {
    /* Waiting reply from probe */
    while((status = tlvWriteDataInWord(_session, address, writeData)) == 0) {
      tlvService(_session);
    };
  } Catch(err) {
    return err;
  }
  
  return 1;
}

int memoryWriteHalfWord(uint32_t address, uint16_t writeData) {
  // int status = 0;
  
  // if(_session == NULL) _session = tlvCreateSession();
  // /* Start tlv request task */
  // startTask(session->memoryRwState);
  // Try {
    // /* Waiting reply from probe */
    // while((status = tlvWriteDataInHalfWord(_session, address, writeData)) == 0) {
      // yield(session->memoryRwState);
    // };
  // } Catch(err) {
    // return err;
  // }
  // /* End tlv request task */
  // endTask(session->memoryRwState);
  
  // return 1;
}

int memoryWriteByte(uint32_t address, uint8_t writeData) {
  // int status = 0;
  
  // if(_session == NULL) _session = tlvCreateSession();
  // /* Start tlv request task */
  // startTask(session->memoryRwState);
  // Try {
    // /* Waiting reply from probe */
    // while((status = tlvWriteDataInByte(_session, address, writeData)) == 0) {
      // yield(session->memoryRwState);
    // };
  // } Catch(err) {
    // return err;
  // }
  // /* End tlv request task */
  // endTask(session->memoryRwState);
  
  // return 1;
}
#endif