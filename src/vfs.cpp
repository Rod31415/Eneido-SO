#include "headers/vfs.h"

#define STARTCLUSTER 0x200000
#define STARTDATACLUSTER 0x204000

#define MAXDIRECTIONS 4096
#define CLUSTERSIZE 512
#define MAXCLUSTERFILE 16
#define MAXCLUSTERFILESYSTEM 16384

#define DIRPERCLUSTER 8

#define FIRSTDATACLUSTER 512

FILESYSTEM filesystem;

uint32 actualClusterDIR=0;
uint32 actualDirectoryDIR=0;


DIR root;

void writeDirectory(DIR buffer,uint32 cluster,uint32 directory){
    for(uint32 i=0;i<49;i++)
      *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+i)=buffer.name[i];
      *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+49)=buffer.flags;
      *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+50)=buffer.size;
      *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+54)=buffer.firstcluster;
      *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+58)=buffer.directions;
      *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+59)=buffer.nextCluster;
      *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+63)=buffer.nextDirection;

  }
void writeCluster(uint8 buffer[512],uint32 cluster){
for(uint32 i=0;i<512;i++)
      *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+i)=buffer[i];

}
void readDirectory(DIR *buffer,uint32 cluster,uint32 directory){
for(uint32 i=0;i<49;i++)
      buffer->name[i] = *(int8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+i);
      buffer->flags = *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+49);
      buffer->size = *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+50);
      buffer->firstcluster = *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+54);
      buffer->directions = *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+58);
      buffer->nextCluster = *(uint32*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+59);
      buffer->nextDirection = *(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+directory*64+63);
      
  
}
void readCluster(uint8 *buffer[512],uint32 cluster){
for(uint32 i=0;i<512;i++) 
      *buffer[i]=*(uint8*)(STARTCLUSTER+cluster*CLUSTERSIZE+i);

}

void initVFS(){

filesystem.directions=(uint32)STARTCLUSTER;
filesystem.clusters=(uint32)STARTDATACLUSTER;
root.name[0]='.';
root.flags=1;
root.size=0;
root.firstcluster=1;
root.directions=0;
root.nextCluster=0;
root.nextDirection=0;
writeDirectory(root,0,0);

DIR file={"file.txt",0,1,512,0,1,1};
writeDirectory(file,1,0);
file={"new",1,1,20,0,0,0};
writeDirectory(file,1,1);
file={"..",2,1,1,0,20,1};
writeDirectory(file,20,0);

file={"ENEIDO.cpp",0,1,513,0,20,2};
writeDirectory(file,20,1);
file={"LEER.txt",0,1,514,0,0,0};
writeDirectory(file,20,2);

actualClusterDIR=root.firstcluster;
actualDirectoryDIR=0;

}
int readFiles(){
  DIR actualDir={};
  int i=0;
  uint32 aux1=actualClusterDIR,aux2=actualDirectoryDIR;
  do{
    i++;
    readDirectory(&actualDir,actualClusterDIR,actualDirectoryDIR);
    actualClusterDIR=actualDir.nextCluster;
    actualDirectoryDIR=actualDir.nextDirection;
  if(i!=1)printf("/n");
  changeColor(actualDir.flags==0?0x0f:0x03);
  printf("-");
  printf(actualDir.name);
  }while(actualClusterDIR!=0);
  actualClusterDIR=aux1;
  actualDirectoryDIR=aux2;
return i;


}

int changeDirectory(char* name){
int a=0;
  uint32 aux1=actualClusterDIR,aux2=actualDirectoryDIR;
DIR actualDir={};
  do{
    readDirectory(&actualDir,actualClusterDIR,actualDirectoryDIR);
    
    actualClusterDIR=actualDir.nextCluster;
    actualDirectoryDIR=actualDir.nextDirection;
if(strcmp(actualDir.name,name,0)==0){a=a|1;actualClusterDIR=actualDir.firstcluster;actualDirectoryDIR=0;break;}
else{a=a|0;}
  }while(actualClusterDIR!=0);
  if(a==0){actualClusterDIR=aux1;
  actualDirectoryDIR=aux2;}
return a;


}


