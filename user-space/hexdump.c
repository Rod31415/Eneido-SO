#include <stdio.h>
#include <stdbool.h>

void print_hex(unsigned char* buffer, int num) {
    /*for (int i = 1; i < num; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }

        printf("%.2X ", buffer[i-1]);
    }*/

    printf("\n");
}

int main(int argc, char* argv[]) {
    setvbuf(stdout,NULL,_IONBF,0);
  if(argc<2)return -1;
  FILE *f=fopen(argv[1],"r");
  if(f==0){return -1;fclose(f);}
  char buf[1024];
  int num=0;
  while((buf[num++]=fgetc(f))!=EOF);

  for (int i = 0; i < num; i++) {
        if (i % 10 == 0&&i!=0) {
            printf(" ");
		for(int j=0;j<10;j++){
	    		printf("%c ",buf[i-10+j]);
	    	}
		printf("\n");
        }

        printf("%.2X ", buf[i]);
    }

    printf("\n");
  
  fclose(f);
  return 0;
}
