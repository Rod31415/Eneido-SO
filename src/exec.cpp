#include "headers/exec.h"

#define TOTALEXECAVAIL 2

char EXECS[TOTALEXECAVAIL][49] = {
    "snake.exe",
    "calculadora.exe"};

void calculadora() {}

int comprobeExec(char *name)
{
  uint32 foundExec = TOTALEXECAVAIL + 1;
  for (uint16 i = 0; i < TOTALEXECAVAIL; i++)
  {
    if (strcmp(EXECS[i], name) == 0)
    {
      foundExec = i;
      break;
    }
  }
  if (foundExec == TOTALEXECAVAIL + 1)
  {
    return -1;
  }
  switch (foundExec)
  {
  case 0:
    snake();
  case 1:
    calculadora();
  }
  return 0;
}
