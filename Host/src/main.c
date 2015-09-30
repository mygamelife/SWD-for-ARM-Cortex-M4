#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  
  Tlv_Session *session = tlvCreateSession();
  
  displayOptionMenu();
  
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