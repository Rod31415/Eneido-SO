#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 512
#define MAX_LINES 1024

// ---- Funciones ----
int cargarArchivo(const char *filename, char lines[][MAX_LINE_LEN], int max_lines);
void mostrarLineas(char lines[][MAX_LINE_LEN], int line_count);
int elegirLinea(int line_count);
void modificarLinea(char lines[][MAX_LINE_LEN], int linea);
int guardarArchivo(const char *filename, char lines[][MAX_LINE_LEN], int line_count);

// ---- main ----
int main(int argc, char *argv[]) {

    char lines[MAX_LINES][MAX_LINE_LEN];
    int line_count;

    line_count = cargarArchivo(argv[1], lines, MAX_LINES);
    if (line_count < 0) return 1;
    printf("HOLA\n");

    return 1;
    mostrarLineas(lines, line_count);

    int linea = elegirLinea(line_count);
    if (linea == -1) {
        return 1;
    }

    modificarLinea(lines, linea);
    guardarArchivo(argv[1], lines, line_count);

    printf("\nArchivo actualizado correctamente.\n");
    return 0;
}

// ---- Implementación de funciones ----

// Carga el archivo en memoria
int cargarArchivo(const char *filename, char lines[][MAX_LINE_LEN], int max_lines) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return -1;
    }

    int count = 0;
    while (fgets(lines[count], MAX_LINE_LEN, f) && count < max_lines) {
        count++;
    }
    fclose(f);
    return count;
}

// Muestra el archivo numerado
void mostrarLineas(char lines[][MAX_LINE_LEN], int line_count) {
    printf("\nContenido del archivo:\n");
    for (int i = 0; i < line_count; i++) {
        printf("%3d: %s", i + 1, lines[i]); // cada línea ya incluye \n
    }
}

// Pregunta al usuario qué línea modificar
int elegirLinea(int line_count) {
    int choice;
    printf("\nIngrese el numero de linea a modificar (1-%d): ", line_count);
    if (scanf("%d", &choice) != 1 || choice < 1 || choice > line_count) {
        fprintf(stderr, "Linea invalida\n");
        return -1;
    }
    getchar(); // limpiar el \n que deja scanf
    return choice - 1; // devolver índice
}

// Modifica la línea elegida
void modificarLinea(char lines[][MAX_LINE_LEN], int linea) {
    char buffer[MAX_LINE_LEN];

    printf("Nuevo contenido para la linea %d: ", linea + 1);
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        fprintf(stderr, "Error leyendo nueva linea\n");
        return;
    }

    // Quitar \n final
    buffer[strcspn(buffer, "\n")] = '\0';

    // Reemplazar en el arreglo (agregamos \n al final)
    snprintf(lines[linea], MAX_LINE_LEN, "%s\n", buffer);
}

// Guarda el archivo
int guardarArchivo(const char *filename, char lines[][MAX_LINE_LEN], int line_count) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        perror("No se pudo abrir archivo para escribir");
        return -1;
    }

    for (int i = 0; i < line_count; i++) {
        fputs(lines[i], f);
    }
    fclose(f);
    return 0;
}