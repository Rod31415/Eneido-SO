#include "headers/syscall.h"

uint32 colors[8]={0xFF000000, //0x0
     0xFFA80000, //0x1
   0xFF00A800, //0x2
 0xFFA8A800, //0x3
    0xFF0000A8, //0x4
  0xFFA800A8, //0x5
    0xFF0057A8, //0x6
   0xFFA8A8A8, //0x7
};


int syscall_readInd3(int fd, char *buf, int count)
{
	if(fd!=0){

		kgets(buf,count,(KFILE*)fd);
		return count;
	}

		uint8 ch=0;
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

char* parseANSI(char* buffer){
	buffer+=2;
	char seq[64];
	int auxIndex=0;
	while(*buffer&&!isAlpha(*buffer)&&auxIndex<63){
		seq[auxIndex++]=*buffer++;
	}
	char command =*buffer++;
	changeColor(colors[seq[1]-48]);
	return buffer;
}

int syscall_writeInd4(int fd, char *buf, int count)
{

	if(fd!=1){
		kputs(buf,count,(KFILE*)fd);
		return count;
	}

bool inEscape=false;

		for (char* buffer=buf; buffer-buf < count; buffer++)
		{
			if(inEscape){
				buffer=parseANSI(buffer);
				inEscape=false;
			}
			else{
				if(*buffer==033){
					inEscape=true;
				}
				else{
					printChr(*buffer);
				}
			}
		}
		refresh();

	return count;
}


int syscall_openInd5( char* name,int flags, uint32 mode)
{
	KFILE* f=kopen(name,flags);
	return (int)f;
}

int syscall_closeInd6(KFILE* f)
{
	return kclose(f);
}

int syscall_seekInd19(int fd, int offset, int mode)
{
	kseek(offset,(KFILE*)fd);
	return offset;
}

int syscall_mkdirInd39(const char* name, uint32 mode)
{
	char buffer[12]="Nuevo     ";
	//memset((uint32)buffer,0,12);
	//uint32 l=lenghtStr(name);
	//memcpy((uint32)name,(uint32)buffer,l);
	DIR d=fCreateNewDirectory(buffer);
	if(d==(DIR)-1)return -1;
	return 0;
}


int syscall_writevInd146(unsigned long fd, const struct iovec *vec, unsigned long vlen)
{
	int total = 0;
	for (int i = 0; i < vlen; i++)
	{
		total += syscall_writeInd4(fd, (char *)vec[i].iov_base, vec[i].iov_len);
	}
	return total;
}