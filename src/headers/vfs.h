#ifndef VFS_H
#define VFS_H
#include "types.h"
#include "functions.h"
#include "string.h"
typedef struct{
uint32 directions;
uint32 clusters;
}__attribute__((packed)) FILESYSTEM;


void readCluster(uint8 *buffer,uint32 cluster);
void writeCluster(uint8 buffer[512],uint32 cluster);
typedef struct{

int8 name[49];
uint8 flags;
uint32 size;
uint32 dataCluster;
uint8 dataDirection;
uint32 nextCluster;
uint8 nextDirection;
void read(uint8* buffer){
readCluster(buffer,dataCluster*8+dataDirection);
  }
void write(uint8* buffer){
writeCluster(buffer,dataCluster*8+dataDirection);
}

}__attribute__((packed)) DIR;

void initVFS();
void readDirectory(DIR* buffer,uint32 cluster,uint32 directory);
void writeDirectory(DIR buffer,uint32 cluster,uint32 directory);

int readFiles();
DIR searchFile(char *name);
int changeDirectory(char *name);
int createDirectory(char *name);
int modifyFile(char *name,DIR File);
int createFile(char *name);

#endif
