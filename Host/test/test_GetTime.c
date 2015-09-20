#include "unity.h"
#include "GetTime.h"
#include <sys/time.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_getCurrentTime_should_get_elapsedTime(void)
{ 
  int i;
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
  
  startTimer();
  for(i = 0; i < 500000; i++);
  getElapsedTime();
}