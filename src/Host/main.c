#include "ProgramLoader.h"
#include <stdlib.h>

int main(void) {
  CEXCEPTION_T err;
  static Tlv_Session *session = NULL;
  static Tlv *reply = NULL;
  
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
      tlvService(session);
      hostInterpreter(session);
      reply = tlvReceive(session);
      if(reply != NULL) {
        reply = NULL;
        printf("Full Erase Done....\n");
        printf("> ");
      }
    } Catch(err) {
      displayErrorMessage(err);
    }
  }
  
  printf("Closing port\n");
  closePort(session);

  return 0;
}