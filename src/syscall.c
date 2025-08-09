#include "headers/syscall.h"

int syscall_writeInd3(int fd, char *buf, int count)
{
	
	if(fd!=0)return -1;

		uint8 ch;
		eatBuffer();
		int i=0;
		while (1)
		{
			ch = getLastAsciiKey();
			if (!isKeyBuffered()||ch ==0)
				continue;
			eatKeyBuffered();
			
			if (ch == 0x80){
				buf[i++]='\n';
				printf("%c", 0xA);
				refresh();
				break;
			}
			printf("%c", ch);
			buf[i++] = (char)ch;
			refresh();

		}
	refresh();
	return i;
}

int syscall_writeInd4(int fd, const char *buf, int count)
{

	if (fd == 1)
	{
		for (uint32 i = 0; i < count; i++)
		{

			printChr(buf[i]);
		}
		refresh();
	}
	return count;
}

int syscall_writevInd146(unsigned long fd, const struct iovec *vec, unsigned long vlen)
{
	int total = 0;
	for (int i = 0; i < vlen; i++)
	{
		total += syscall_writeInd4(fd, (const char *)vec[i].iov_base, vec[i].iov_len);
	}
	return total;
}