#include <fcntl.h>
 #include <string.h>
 #include <unistd.h>
 #include <stdio.h>
 
 
int main(int argc, char* argv[])
{

    if(argc<2)return -1;
    char name[11];
    strcpy(name,argv[1]);
    printf(" creado exitosamente");
    FILE* f=fopen(name,"w");

    if(f==0){fclose(f);return -1;}

    fclose(f);
    return 1;
}