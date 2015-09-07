#include "ProgramLoader.h"
#include <stdlib.h>

// #define HOST

int main(void) {
  Tlv_Session *session = tlvCreateSession();
  printf("Opening port\n");
  
  
  displayOptionMenu();
  printf("Please Enter [--Option--] [--Parameters--]\n");
  printf("Example : 1 R0 0xDEADBEFF\n\n");
  
  while(session->hostState != HOST_EXIT) {
    tlvService(session);
    hostInterpreter(session);
  }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session->handler;
  closeSerialPort(hSerial);

  return 0;
}