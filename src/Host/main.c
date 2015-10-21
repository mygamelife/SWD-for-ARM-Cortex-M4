#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  Tlv_Session *session = NULL;
  
  displayOptionMenu();
  
  Try {
    if(session == NULL) session = tlvCreateSession();
    while(session->hostState != HOST_EXIT) {
      Try {
        tlvService(session);
        hostInterpreter(session);
      } Catch(err) {
        session->hostState = HOST_WAIT_USER_COMMAND;
        displayErrorMessage(err);
      }
    }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  printf("Closing port\n");
  closeSerialPort(session->handler);

  return 0;
}