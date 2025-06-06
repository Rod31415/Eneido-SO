#include "headers/string.h"


int8 strcmp(int8 *str1, int8 *str2, uint16 size){
  uint32 i=0,ptot=0;
if(size==0){
  
  while(str1[i]!=0||str2[i]!=0){
    if(str1[i]==str2[i])ptot++;
    if(str1[i]==0)return 1;
    else if(str2[i]==0)return -1;
      i++;
  }
  if(ptot==i)return 0;
  else return 10;

}
for(i=0;i<size;i++){

 if(str1[i]==str2[i])ptot++;
    if(str1[i]==0)return 1;
    else if(str2[i]==0)return -1;
      
  }
  if(ptot==i)return 0;
  else return 10;




}

