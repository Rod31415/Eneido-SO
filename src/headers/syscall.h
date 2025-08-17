#ifndef SYSCALL_H
#define SYSCALL_H
#include "functions.h"
#include "types.h"
#include "vfs.h"

struct iovec
  {
    void *iov_base;	/* Pointer to data.  */
    long unsigned int iov_len;	/* Length of data.  */
  };

int syscall_readInd3(int fd,char *buf, int count);
int syscall_writeInd4(int fd,char *buf, int count);
int syscall_openInd5(char* name,int flags, uint32 mode);
int syscall_closeInd6(KFILE* f);
int syscall_seekInd19(int fd, int offset, int mode);
int syscall_mkdirInd39(const char* name, uint32 mode);
int syscall_writevInd146(unsigned long fd,const struct iovec *vec,unsigned long vlen);


#endif