#include "headers/string.h"

uint32 lenghtStr(int8 *str){
uint32 length=0;
    while (str[length] != 0)
    {
      length++;
    }
  return length;
}

void eraseStr(int8 *str,uint32 index,uint32 num){
if(index+num>lenghtStr(str))return;

for(int i=index;i<lenghtStr(str);i++)
  {
  str[i]=str[i+num];
  }
}

void insertStr(int8 *str,uint32 index, int8 character){

if(index>lenghtStr(str))return;

for(int i=lenghtStr(str);i>index;i--)
  {
  str[i]=str[i-1];
  }
str[index]=character;

}


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

  while(*str1 &&*str1==*str2){str1++;str2++;}
  return (*str1>*str2)-(*str1<*str2);

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
