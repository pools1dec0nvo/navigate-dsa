#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

/***********************************************************************
 * check_file_extension
 * Descrição: Verifica se o nome do ficheiro fornecido tem a extensão especificada.
 * Argumentos:
 *   - filename: nome do ficheiro a verificar.
 *   - extension: extensão a comparar.
 * Devolve:
 *   - 1 se a extensão coincide; 0 caso contrário.
 ***********************************************************************/
int check_file_extension(char *filename, char *extension) {
    size_t filenameLength = strlen(filename);
    size_t extensionLength = strlen(extension);

    if (filenameLength >= extensionLength) {
        if (strcmp(&filename[filenameLength - extensionLength], extension) == 0) {
            return 1;
        }
    }
    return 0;
}
/***********************************************************************
 * change_file_extension
 * Descrição: Altera a extensão de um ficheiro de entrada para uma extensão especificada.
 * Argumentos:
 *   - input: nome do ficheiro de entrada.
 *   - output: buffer onde será armazenado o novo nome do ficheiro.
 *   - extension: nova extensão a ser aplicada.
 ***********************************************************************/
void change_file_extension(char* input, char* output, char* extension) {
    char* dot = strrchr(input, '.');
    if (dot == NULL) {
        dot = input + strlen(input);
    }
    strncpy(output, input, dot - input);
    output[dot - input] = '\0';
    strcat(output, extension);
}
