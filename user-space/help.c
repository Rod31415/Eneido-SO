#include <stdio.h>

int main(int argc,char *argv[]){
    printf("\033[32m  Solo tenemos los siguientes comandos: \n \n");
	printf("   echo <mensaje>             - Muestra un mensaje en la pantalla \n");
	printf("   init                       - Reinicia la terminal \n");
	printf("   help                       - Abre esta ventana \n");
	printf("   colors                     - Abre una prueba de colores \n");
	printf("   license                    - Muestra la licencia y creditos del proyecto \n");
	printf("   ls                         - Lista los archivos del directorio \n");
	printf("   tree                       - Lista los archivos de todo el FS \n");
	printf("   cd <directorio>            - Cambia de directorio \n");
	printf("   mkdir <directorio>         - Crea un directorio \n");
	printf("   inf <archivo>              - Muestra la informacion del archivo \n");
	printf("   cat <archivo>              - Muestra el contenido del archivo \n");
	printf("   touch <archivo>            - Crea un archivo \n");
	printf("   edit <archivo> <contenido> - Edita un archivo \n");
	printf("   edit <archivo>             - Abre el archivo con el editor de texto\n");
	printf("   date                       - Muestra la fecha y hora actual\n");
	printf("   pci                        - Muestra los dispositivos PCI\n");
	printf("   doen                       - Ejecuta el interprete de DOEN\n");
	printf("   send 'file' <a|archivo>    - Envia el argumento o archivo por la red local\n");
	printf("   recv                       - Recibe el ultimo paquete enviato por la red local\n");
	printf("   mac                        - Mustra la direccion MAC\n");
	printf("   elf <archivo>              - Enlaza y ejecuta programas en base a un archivo elf \n");
	printf("   view <archivo>             - Muestra imagenes de los modulos de GRUB \n");
	return 0;
}