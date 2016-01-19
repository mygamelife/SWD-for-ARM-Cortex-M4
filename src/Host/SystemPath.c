#include "SystemPath.h"

char currentPath[FILENAME_MAX];
char *buildRelease = "\\build\\release";

int isCurrentDirectory(char *cwd) {
  assert(cwd != NULL);
  
  char *cwdPtr = strstr(cwd, buildRelease);

  if(cwdPtr != NULL) {
    if(strcmp(cwdPtr, buildRelease) == 0) {
      return 1;
    }
  }
  else {
    return 0;
  }
}

char *getCurrentDirectory(void) {
  if(GetCurrentDir(currentPath, sizeof(currentPath))) {
    return currentPath;
  }
  else {
    printf("Can't find current working directory\n");
    return NULL;
  }
}

/** getFpPath is a function to acquire the FlashProgrammer file path
  * based on the current directory file path.
  *
  * input     : cwd is the current working directory path can be
  *             any file path
  *
  * return    : FP_INSIDE_CWD if inside current directory
  *             FP_OUTSIDE_CWD if outisde from the current directory
  */
char *getFpPath(char *cwd) {
  assert(cwd != NULL);
  
  if(isCurrentDirectory(cwd))
  {
    return FP_INSIDE_CWD;
  }
  else
  {
    return FP_OUTSIDE_CWD;
  }
}