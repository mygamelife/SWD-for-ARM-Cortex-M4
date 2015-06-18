#include "Flash.h"

/**
 * Access CTRL/STATUS register by sending SWD-DP request
 * DP = 0, AP = 1
 * WRiTE = 0, READ = 1
 * Parity Odd 1's = 1, Parity Even 1's = 0
 ******************************************************************************************************
 |  Start bit   | 	APnDP   |   RW     |    Addr2   |   Addr3   |   Parity    |   Stop    |   Park    |
 ------------------------------------------------------------------------------------------------------
 |      1       |    0      |    1     |     0      |     0     |     1       |    0      |     1     |
 ******************************************************************************************************
 *
 * Input : Address_bit3 is the address bit 3 of the AP/DP register
 *		   Address_bit2	is the address bit 2 of the AP/DP register
 *         APnDP is the Access Port or Debug Port access
 *         ReadWrite is the Read or Write access
 *
 * Output : return the value of parity calculated
 */
void access_CtrlStatusReg() {
  int request;
  //request = getSWD_Request(0x4,DP,WRITE);
}