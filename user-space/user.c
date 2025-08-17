#include <stdio.h>



int main(int argc,char *argv[]){
  setvbuf(stdout,NULL,_IONBF,0);
  if(argc<2)return -1;
  FILE *f=fopen(argv[1],"r");
  if(f==0){return -1;fclose(f);}
  char buf[80];
  int i=0;
  while((buf[i++]=fgetc(f))!=EOF);
  
  printf("%s",buf);
  fclose(f);
  return 0;
  
}