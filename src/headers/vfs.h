#ifndef VFS_H
#define VFS_H
#include "types.h"
#include "functions.h"
#include "string.h"
#include "utilities.h"
#include "keyboard.h"
#include "multiboot.h"
#include "mem.h"


#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR   0x0002
#define O_CREAT  0x0040
#define O_EXCL   0x0080
#define O_APPEND 0x0400
#define O_ASYNC  0x2000
#define O_CLOEXEC 0x80000
#define O_NONBLOCK 0x0800
#define O_TMPFILE   0x410000

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
typedef int32 POSFILE;

typedef struct{

int used;
DIR entry;
POSFILE pos;
uint32 size;
uint32 flags;


}__attribute__((packed)) KFILE;





void initRootDirectoryWithModules(multiboot_info *mb);
char* parseFATnames(char* filename);
bool changeDirectory(char* str);
void listDirectories();
void seeAllVFSFromClusterP(CLUSTER P,uint32 stack);
DIR retBinDirectory();

DIR fCreateNewDirectory(char* str);
DIR fCreateNewFile(char* str);


KFILE* kopen(char* filename,uint32 flags);
bool kclose(KFILE* file);
DIR kopendir(DIR directory,char* filename);
void kputs(char* data, uint32 lenght, KFILE* f);
void kgets(char* data, uint32 lenght, KFILE* f);
void* kgetpointer(KFILE* f);
void kseek(int32 point,KFILE* f);
#endif
