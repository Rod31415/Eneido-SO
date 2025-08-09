#ifndef VFS_H
#define VFS_H
#include "types.h"
#include "functions.h"
#include "string.h"
#include "utilities.h"
#include "keyboard.h"
#include "multiboot.h"

typedef struct{

char   Name[11];
uint8  Attr;
uint8  NTRes;
uint8  CrtTimeTenth;
uint16 CrtTime;
uint16 CrtDate;
uint16 LstAccDate;
uint16 FstClusHI;
uint16 WrtTime;
uint16 WrtDate;
uint16 FstClusLO;
uint32 FileSize;

}__attribute__((packed)) DirectoryStruct;

typedef uint32 CLUSTER;
typedef uint32 ENTRY;

typedef DirectoryStruct* DIR;
typedef DirectoryStruct FILE;
typedef int32 POSFILE;

void initRootDirectoryWithModules(multiboot_info *mb);
char* parseFATnames(char* filename);
bool changeDirectory(char* str);
void listDirectories();
void seeAllVFSFromClusterP(CLUSTER P,uint32 stack);
DIR retBinDirectory();

FILE* fCreateNewDirectory(char* str);
FILE* fCreateNewFile(char* str);


FILE* fopen(char* filename);
FILE* fopendir(DIR directory,char* filename);
void fputs(char* data, FILE* f);
void fgets(char* data, uint32 lenght, FILE* f);
void* fgetpointer(FILE* f);
void fseek(int32 point,FILE* f);
#endif
