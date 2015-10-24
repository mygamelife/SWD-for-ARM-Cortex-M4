#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  Tlv_Session *session = NULL;
  
  displayOptionMenu();
  
  Try {
    if(session == NULL) session = tlvCreateSession();
    while(!IS_HOST_EXIT(session)) {
      Try {
        tlvService(session);
        hostInterpreter(session);
        // 
        // Receive packet and handle it here
        //
      } Catch(err) {
        HOST_CHANGE_STATE(session, HOST_WAIT_USER_COMMAND);
        // session->hostState = HOST_WAIT_USER_COMMAND;
        displayErrorMessage(err);
      }
    }
  } Catch(err) {
    displayErrorMessage(err);
  }
  
  printf("Closing port\n");
  closePort(session);

  return 0;
}