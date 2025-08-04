#ifndef VFS_H
#define VFS_H
#include "types.h"
#include "functions.h"
#include "string.h"
#include "utilities.h"
#include "keyboard.h"

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

typedef DirectoryStruct* DIR;
typedef DirectoryStruct FILE;
typedef int32 POSFILE;

void initRootDirectory();



FILE* fopen(char* filename);
void fputs(char* data, FILE* f);
void fgets(char* data, uint32 lenght, FILE* f);
void fseek(int32 point,FILE* f);
#endif
