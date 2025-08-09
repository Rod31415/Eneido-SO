#ifndef SYSCALL_H
#define SYSCALL_H
#include "functions.h"
#include "types.h"

struct iovec
  {
    void *iov_base;	/* Pointer to data.  */
    long unsigned int iov_len;	/* Length of data.  */
  };

int syscall_writeInd3(int fd,char *buf, int count);
int syscall_writeInd4(int fd, const char *buf, int count);
int syscall_writevInd146(unsigned long fd,const struct iovec *vec,unsigned long vlen);


#endif