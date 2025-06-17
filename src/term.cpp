#include "headers/term.h"

uint8 boolOnTest = 0;
uint16 selectedLine=0;
uint16 actualLine=0;
int8 linesHistory[200][80];
uint32 flags, memLow, memHigh, sect;

void drawOpenSource()
{
  gotoxy(0,0);
  changeColor(0x02);
  printf("          #####/n");
  printf("      #############/n");
  printf("    #################/n");
  printf("  #####################/n");
  printf("  #########   #########/n");
  printf("#########       #########/n");
  printf("#########       #########/n");
  printf("#########       #########/n");
  printf("  ########     ########/n");
  printf("  ########     ########/n");
  printf("    #####       #####/n");
  printf("      ###       ###");

  gotoxy(41, 1);
  printf("ENEIDO SO ");
  gotoxy(41, 2);
  printf(" 32 bits ");
  gotoxy(34, 4);

  changeColor(0x0E);
  printf("   Por Rodrigo Bustos ");
  changeColor(0x02);
  gotoxy(0, 13);
  printf(" Memoria Disponible: %d KB /n", memLow + memHigh);
  printf(" Banderas: %d /n /n", flags);
  changeColor(0x0f);
  printf(" Escribi 'help' para ver los comandos/n");
}

uint32 xCursor = 0, yCursor = 0;
uint8 ch = 0, s = 0;
uint8 character = 0;
int8 line[80];
uint8 indexLetter = 0;
multiboot_info *mboot;
uint32 argc;
int8 argv[10][40];
uint8 first=0;

void init_term(multiboot_info *mb)
{
  if(first==0){
  mboot = mb;
  flags=mboot->flags;
  memLow=mboot->mem_lower;
  memHigh=mboot->mem_upper;
  sect=mboot->mods_count;}
  cls(0x00);
  first=1;
  drawOpenSource();
  gotoxy(0, 17);
  changeColor(0x0f);
  printf(">");
  xCursor = 1;
  yCursor = 17;
  update_cursor(xCursor, yCursor);
refresh();    
}

void formatCommand()
{
  argc = 1;
  uint8 active = 1;
  uint8 comm = 0;
  int i;
  for (int j = 0; j < 10; j++)
  {
    for (i = 0; i < 80; i++)
    {
      argv[j][i] = 0;
    }
  }
  i = 0;
  int a = 0;

  uint8 cn = 0;
  do
  {
    cn = 0;
    if (line[i] == '"')
    {
      comm = !comm;
    }
    else if (line[i] == ' ' && !comm)
    {
      if (active == 0)
      {
        argc++;
        active = 1;
      }
      cn = 1;
      a = i + 1;
    }

    else
    {
      active = 0;
    }
    if (!cn)
      argv[argc - 1][i - a] = line[i];

  } while (line[++i] != 0);
}

void echo()
{
  backspace();
  for (int i = 0; i < 80; i++)
  {
    (argv[1][i] == '"') ? argv[1][i] = ' ' : 0;
  }
  printf((argc == 2) ? argv[1] : "ERROR : Escribe un mensaje despues del comando");
  new_line_term();
}

void help()
{
  backspace();
  changeColor(0x07);
  printf("  Solo tenemos los siguientes comandos: /n /n");
  changeColor(0x0E);
printf("  Solo tenemos los siguientes comandos: /n /n");
  printf("   echo <mensaje>             - Muestra un mensaje en la pantalla /n");
  printf("   init                       - Reinicia la terminal /n");
  printf("   help                       - Abre esta ventana /n");
  printf("   colors                     - Abre una prueba de colores /n");
  printf("   license                    - Muestra la licencia y creditos del proyecto /n");
  printf("   ls                         - Lista los archivos del directorio /n");
  printf("   cd <directorio>            - Cambia de directorio /n");
  printf("   mkdir <directorio>         - Crea un directorio /n");
  printf("   inf <archivo>              - Muestra la informacion del archivo /n");
  printf("   cat <archivo>              - Muestra el contenido del archivo /n");
  printf("   touch <archivo>            - Crea un archivo /n");
  printf("   edit <archivo> <contenido> - Edita un archivo /n");
  printf("   edit <archivo>             - Abre el archivo con el editor de texto/n");
  printf("   clusters                   - Abre la ventana del sistema de archivos /n");
  printf("   games                      - Abre la ventana de juegos /n");
  new_line_term();
}

void colors()
{
  boolOnTest = 1;

  cls(0x00);
  gotoxy(0, 0);
  for (uint32 i = 0; i < 256; i++)
  {

    for(uint32 x=0;x<20;x++){
    for(uint32 y=0;y<12;y++){
      draw_pixel((i%16)*20+x,(i/16)*12+y,i);
    }
  }
}
}

void license()
{
  backspace();
  printf("/n   GNU General Public License v3.0 /n");
  printf("     Hecho por Rodrigo Bustos /n");
  printf(" Puedes acceder al codigo fuente yendo a :/n");
  printf("   https://github.com/Rod31415/Eneido-SO/n /n");
  new_line_term();
}

void clear()
{
  cls(0x00);
  gotoxy(0, 0);
  changeColor(0x0f);
  printf(">");
}

void cat(char *name){
backspace();
DIR file=searchFile(name);
if(file.name[0]==0){return;}
uint8 buffer[512];
readCluster(buffer,file.firstcluster*8+file.directions);
char buff[80];
for(int i=0;i<80;i++){
buff[i]=(int8)buffer[i];
}
buff[80]=0;
printf(buff);

}


void inf(char *name){
backspace();
DIR file=searchFile(name);
if(file.name[0]==0){return;}
printf("Nombre: ");
printf(file.name);
printf("/nBanderas: %d = ",file.flags);
printf((file.flags==1)?"Archivo":((file.flags==2)?"Directorio":"Retorno"));
printf("/nTamanio: %d clusters",file.size);
printf("/nPrimer Cluster: %d ",file.firstcluster);
printf("/nPrimer Direccion: %d ",file.directions);
}

void edit(char *name,char *str){
DIR file=searchFile(name);
if(file.name[0]==0){return;}
for (int i = 0; i < 80; i++)
  {
    (str[i] == '"') ? str[i] = ' ' : 0;
  }
uint8* buffer=(uint8 *)str;
writeCluster(buffer,file.firstcluster*8+file.directions);
}

void editor(char *name){
DIR file=searchFile(name);
if(file.name[0]==0){return;}
initEditor(file);
}


void clusters(){

 boolOnTest = 1;

  cls(0x00);
  uint8 color=0;
  DIR file;
  for(uint32 j=0;j<4000;j++){
  for (uint32 i = 0; i < 8; i++)
  {
    readDirectory(&file,j,i);
    switch (file.flags) {
      case 0 :color=0;break;
      case 1 :color=1;break;
      case 2 :color=2;break;
      case 3 :color=4;break;
      case 10:color=0xf;break;
    }
    for(uint8 x=0;x<4;x++)
    for(uint8 y=0;y<4;y++)
    draw_pixel(((j*8+i)%80)*4+x,((j*8+i)/80)*4+y,color);
    

  }

  }
/*
  changeColor(0x00);
  gotoxy(0,23);
  printf("  %c:Cluster vacio ",219);
  
  changeColor(0x00);
  printf("%c",219);
  changeColor(0x70);
  printf(":Cluster archivo ");
  
  changeColor(0x72);
  printf("%c",219);
  changeColor(0x70);
  printf(":Cluster directorio /n");
  changeColor(0x74);

  printf("  %c",219);
  changeColor(0x70);
  printf(":Cluster retorno ");
  changeColor(0x7f);
  printf("%c",219);
  changeColor(0x70);
  printf(":Cluster datos                  ");*/
}



void detectCommands()
{
  if (argv[0][0] == 0)
  {
    return;
  }


  if (argc == 1 && strcmp(argv[0], "init") == 0)
  {
    init_term(mboot);
  }

  else if (strcmp(argv[0], "echo") == 0)
  {
    echo();
    xCursor = 1;
    if (yCursor < 24)
      yCursor++;
    update_cursor(xCursor, yCursor);
  }

  else if (argc == 1 && strcmp(argv[0], "help") == 0)
  {
    help();
    xCursor = 1;
    for (int i = 0; i < 18; i++)
      if (yCursor < 24)
        yCursor++;

    update_cursor(xCursor, yCursor);
  }

  else if (argc == 1 && strcmp(argv[0], "colors") == 0)
  {
    colors();
  }

  else if (argc == 1 && strcmp(argv[0], "license") == 0)
  {
    license();
    xCursor = 1;

    for (int i = 0; i < 7; i++)
      if (yCursor < 24)
        yCursor++;

    update_cursor(xCursor, yCursor);
  }
  else if (argc == 1 && strcmp(argv[0], "clear") == 0)
  {
    clear();
    xCursor = 1;
    yCursor = 0;
    update_cursor(xCursor, yCursor);
  }

  else if (argc == 1 && strcmp(argv[0], "ls") == 0)
  {
    backspace();
    int h = readFiles();
    changeColor(0x0f);
    xCursor = 1;

    for (int i = 0; i < h; i++)
      if (yCursor < 24)
        yCursor++;
    update_cursor(xCursor, yCursor);

    new_line_term();
  }

  else if (argc == 2 && argv[1][0] != 0 && strcmp(argv[0], "cd") == 0)
  {

    if (changeDirectory(argv[1]) == 0)
    {
      backspace();
      printf(" Carpeta no encontrada ");
      if (yCursor < 24)
        yCursor++;
      update_cursor(xCursor, yCursor);
      new_line_term();
    }
  }

  else if (argc == 2 && strcmp(argv[0], "mkdir") == 0)
  {

    createDirectory(argv[1]);

    update_cursor(xCursor, yCursor);
  }

  else if (argc == 2 && strcmp(argv[0], "cat") == 0){
cat(argv[1]);

 if (yCursor < 24)
        yCursor++;
      update_cursor(xCursor, yCursor);
      new_line_term();

  }
  else if(argc==2&&strcmp(argv[0],"inf")==0){
inf(argv[1]);
    for (int i = 0; i < 5; i++)
 if (yCursor < 24)
        yCursor++;
      update_cursor(xCursor, yCursor);
      new_line_term();

  }
 else if (argc == 2 && strcmp(argv[0], "touch") == 0)
  {

    createFile(argv[1]);

    update_cursor(xCursor, yCursor);
  }

  else if(argc==3 && strcmp(argv[0],"edit")==0){
    edit(argv[1],argv[2]);
    update_cursor(xCursor,yCursor);

  }
  else if(argc==2 && strcmp(argv[0],"edit")==0){
    editor(argv[1]);
    init_term(mboot);
  }

  else if(argc==1 && strcmp(argv[0],"clusters")==0){
    clusters();
  }

  else if(argc==1 && strcmp(argv[0],"games")==0){
    initGameHub();
    init_term(mboot);
  }
  else
  {
    backspace();
    printf("Uhh me mataste, no tengo el comando ");
    printf((argv[0]));
    new_line_term();
    xCursor = 1;
    if (yCursor < 24)
      yCursor++;
    update_cursor(xCursor, yCursor);
  }

  for (uint8 i = 0; i < 100; i++)
  {
    line[i] = 0;
  }
  changeColor(0x0f);
}

void loop_term()
{

  character = inport(0x60);
  ch = s;
  s = getKeyboardKey(character);
  if(s == ch)return;
  if (s == 132 && actualLine>0){
    resetLine();
    printf("                                     ");
    resetLine();
    selectedLine--;
    printf(">");
    printf(linesHistory[selectedLine]);
    strcpy(line,linesHistory[selectedLine],80);
  }

  if (s == 130 && selectedLine!=actualLine){
    resetLine();
    selectedLine++;
    printf(">");
    printf(linesHistory[selectedLine]);
    strcpy(line,linesHistory[selectedLine],80);
  }

  if (s == 128)
  {
    
    
    if (boolOnTest)
    {
      boolOnTest = 0;
      init_term(mboot);
    }
    new_line_term();
    xCursor = 1;
    if (yCursor < 24)
      yCursor++;
    update_cursor(xCursor, yCursor);
    formatCommand();
    if (argv[0][0] != 0){
strcpy(linesHistory[actualLine],line,80);
    actualLine++;
    selectedLine=actualLine;
    }
    detectCommands();
    
  }
  if (s == 129 && xCursor > 1 && !boolOnTest)
  {

    backspace();
    
    indexLetter--;
    line[indexLetter] = 0;
    xCursor--;
    update_cursor(xCursor, yCursor);
  }
  if (s != 0 && s < 128 && !boolOnTest)
  {
    printf("%c", (char)s);
    line[indexLetter] = s;
    indexLetter++;
    xCursor++;
    update_cursor(xCursor, yCursor);
  }
  refresh();
}

void new_line_term()
{
  printf("/n>");
  indexLetter = 0;
}
