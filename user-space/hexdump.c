#include <stdio.h>
#include <stdbool.h>

void print_hex(unsigned char* buffer, int num) {
    for (int i = 0; i < num; i++) {
        if (i % 10 == 0) {
            printf("\n");
        }

        printf("%.2X ", buffer[i]);
    }

    printf("\n");
}

int main(int argc, char* argv[]) {
    FILE* f = fopen(argv[1], "r");
    unsigned char buffer[128];
    int i=0;
    while((buffer[i++]=fgetc(f))!=EOF);
    print_hex(buffer, i-1);

    /*while (true) {
        char cmd;
        int loc;
        scanf("%c%d", &cmd, &loc);

        if (cmd == 'p') {
            print_hex(buffer + loc, 10);
        }

        if (cmd == 'e') {
            scanf("%x", buffer + loc);
        }

        if (cmd == 's') {
            break;
        }
    }
    
    fclose(f);
    f = fopen(argv[1], "w");
    fwrite(buffer, 1, 1024, f);
    fclose(f);*/
    return 0;
}