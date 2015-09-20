#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  
  Tlv_Session *session = tlvCreateSession();
  printf("Opening port\n");
  
  
  displayOptionMenu();
  printf("Please Enter [--Option--] [--Parameters--]\n");
  printf("Example : 1 R0 0xDEADBEFF\n\n");
  
  while(session->hostState != HOST_EXIT) {
    Try {
      tlvService(session);
      hostInterpreter(session);
    }
    Catch(err) {
      session->hostState = HOST_WAIT_USER_COMMAND;
      displayErrorMessage(err);
    }
  }
  
  printf("Closing port\n");
  HANDLE hSerial = (HANDLE)session->handler;
  closeSerialPort(hSerial);

  return 0;
}