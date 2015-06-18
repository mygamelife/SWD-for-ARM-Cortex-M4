#include "Register_ReadWrite.h"

/***************************************************************
 **                   SWD-DP Register                         **
 ***************************************************************/
 
/******************************************************************************************************
 |  Start bit   |  APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    0     |     0      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************/
 
/**Register Address
 *  Address        READ          Write
 *  --------------------------------------
 *  0x00          IDCODE        ABORT
 *  0x04          CTRL/STAT     CTRL/STAT
 *  0x08          RESEND        SELECT
 *  0x04          RDBUFF        N/A
 */

/**
 *	Action          Value        
 * -------------------------------------------------------------------
 * START         Always  1
 * AP/DP1        DP  = 0,   AP  = 1
 * RW            W   = 0,   R   = 1
 * Addr[2:3]     Refer Register Address
 * Parity        APnDP + RW + Addr2 + Addr3 Even 1's = 0, Odd 1's = 1
 * STOP          Always  1   
 * PARK          Always  1
 */

/**
 * Access CTRL/STATUS register by sending SWD-DP request
 */
void ctrlStatusReg(int RnW) {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x04, DP, RnW);
  send8bit(SWD_Request);
}

/**
 * Set CSYSPWRUPRED(bit30) and CDBGPWRUPREQ(bit 28) bits of CTRL/STATUS Register
 */
void setCtrlStatusReg(uint32_t data) {
  send32bit(data);
}

void readIDCODEReg(int RnW) {
  int SWD_Request;

  SWD_Request = getSWD_Request(0x00, DP, RnW);
  send8bit(SWD_Request);
}
