#include <stdio.h>
#include <string.h>

void edit_line(char* buffer, int current_line) {
  char * auxbuf,*refBuffer=buffer;
    for (int i = 0; i < current_line; i++) {
        buffer = strchr(buffer, '\n');
        if(buffer==NULL){
          auxbuf=strchr(buffer, '\0');
          auxbuf[0]='\n';
          auxbuf[1]='\0';
          auxbuf[2]='\0';
          auxbuf[3]='\0';
          buffer=auxbuf;
        }
          buffer++;
    }

    char* line_end = strchr(buffer, '\n');
    
    char saved[1024] = { 0 };
    if(line_end!=NULL)
    strcpy(saved, line_end);
    scanf("%s", buffer);
    if(line_end!=NULL)
    strcpy(buffer + strlen(buffer), saved);
    buffer[strlen(buffer)+strlen(saved)+1]=0;
}

void main(int argc, char* argv[]) {
    FILE* f = fopen(argv[1], "r");
    char buffer[1024];
    int i=0;
    while((buffer[i++]=fgetc(f))!=EOF);
    fclose(f);
    printf("Contents:\n%s\n", buffer);
    int current_line = 0;
    printf("Linea modif: ");
    fflush(stdout);
    scanf("%d", &current_line);
    printf("Nuevo contenido:\n");
    edit_line(buffer, current_line);
    f = fopen(argv[1], "w");
    fwrite(buffer, strlen(buffer), 1, f);
    fclose(f);
}
