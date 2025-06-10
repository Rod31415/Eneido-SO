#include "headers/string.h"

int8 strcpy(int8 *str1, int8 *str2, uint16 size)
{
  if (size == 0)
  {
    int i = 0;
    while (str2[i] != 0)
    {
      str1[i] = str2[i];
      i++;
    }
  }
  else
  {
    for (uint32 i = 0; i < size; i++)
    {
      str1[i] = str2[i];
    }
  }
}

int8 strcmp(int8 *str1, int8 *str2, uint16 size)
{
  uint32 i = 0, ptot = 0;
  if (size == 0)
  {

    while (str1[i] != 0 && str2[i] != 0)
    {

      if (str1[i] == 0)
        return 1;
      else if (str2[i] == 0)
        return -1;
      i++;
      if (str1[i] == str2[i])
        ptot++;
    }
    if (ptot == i)
      return 0;
    else
      return 10;
  }
  for (i = 0; i < size; i++)
  {

    if (str1[i] == str2[i])
      ptot++;
    if (str1[i] == 0)
      return 1;
    else if (str2[i] == 0)
      return -1;
  }
  if (ptot == i)
    return 0;
  else
    return 10;
}
