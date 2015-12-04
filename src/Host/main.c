#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  Tlv_Session *session = NULL;
  User_Session *userSession = NULL;
  
  displayOptionMenu();
  
  Try {
    if(session == NULL) 
      session = tlvCreateSession();
  } Catch(err) {
    displayErrorMessage(err);
    return 0;
  }
  
  while(!isExit(session)) {
    Try {
      // printf("tlvService\n");
      tlvService(session);
      // printf("hostInterpreter\n");
      hostInterpreter(session);
      // 
      // Receive packet and handle it here
      //
    } Catch(err) {
      displayErrorMessage(err);
    }
  }
  
  printf("Closing port\n");
  closePort(session);

  return 0;
}