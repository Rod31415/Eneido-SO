#include "headers/string.h"

int8 isNumeric(int8 ch){
return (ch>=48&&ch<=57);
}

int8 isAlpha(int8 ch){
return ((ch>=65&&ch<=90)||(ch>=97&&ch<=122));
}

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


int32 toInt(int8* str){
  int32 result=str[0]-48;
  int32 i=1;
  while(isNumeric(str[i])){
    result=result*10+(str[i]-48);
  i++;
  }

return result;
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
