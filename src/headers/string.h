#ifndef STRING_H
#define STRING_H
#include "types.h"
#include "utilities.h"

uint32 lenghtStr(int8* str);
void eraseStr(int8* str,uint32 index,uint32 num);
void insertStr(int8* str,uint32 index,int8 character);
int8 strcmp(int8 *str1,int8 *str2,uint16 size=0);
int8 strcpy(int8 *str1,int8 *str2,uint16 size=0);


#endif
