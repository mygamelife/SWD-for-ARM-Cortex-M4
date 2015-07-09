#include <stdio.h>
#include "assertion.h"

void assert_fail(char *msg, const char *file, int line, const char *func)  {
  printf("MESSAGE: %s\n", msg);
  printf("FILE: %s, LINE: %d, FUNCTION: %s", file, line, func);
  return;
}

void dummyTest()  {
  printf("Dump Error\n");
  Error_Handler("ERROR HAPPEN HERE");
}