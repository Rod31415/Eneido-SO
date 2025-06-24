#ifndef VFS_H
#define VFS_H
#include "types.h"
#include "functions.h"
#include "string.h"
typedef struct{

uint32 directions;
uint32 clusters;


}__attribute__((packed)) FILESYSTEM;


typedef struct{

int8 name[49];
uint8 flags;
uint32 size;
uint32 firstcluster;
uint8 directions;
uint32 nextCluster;
uint8 nextDirection;

}__attribute__((packed)) DIR;

void initVFS();
void readCluster(uint8 *buffer,uint32 cluster);
void readDirectory(DIR* buffer,uint32 cluster,uint32 directory);
void writeCluster(uint8 buffer[512],uint32 cluster);
void writeDirectory(DIR buffer,uint32 cluster,uint32 directory);

int readFiles();
DIR searchFile(char *name);
int changeDirectory(char *name);
int createDirectory(char *name);
int modifyFile(char *name,DIR File);
int createFile(char *name);

#endif
