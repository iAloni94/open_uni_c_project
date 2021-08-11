#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"
#include "label.h"

#define MASK 0xFF /* 00000000 00000000 00000000 11111111 */

FILE* createFile(char* fname, char* extention) {
    int i;
    char ext_diff = strlen(fname) - 3; /* length of ".as" = 3 */
    char* fname_no_extention = calloc(sizeof(char), ext_diff);
    FILE* output_file_ptr;

    for (i = 0; i < (ext_diff); i++) {
        *(fname_no_extention + i) = *(fname + i);
    }
    /* Opening file for output */
    output_file_ptr = fopen(strcat(fname_no_extention, extention), "w");
    free(fname_no_extention);
    return output_file_ptr;
}

void printObj(FILE* fp, unsigned int* codeImg, dir_t* dataImg, unsigned int ICF, unsigned int DCF) {
    unsigned int a, b, c, d, i = 0, j = 0, IC = 100;
    unsigned char* data = calloc(DCF, 1);

    fprintf(fp, "%d\t%d", ICF - 100, DCF);

    /*  print code image*/
    /* shift mask to desired 4 bits location and then shift the result to the rightmost 4 bits for printing */
    for (i = 0; *(codeImg + i) != 0; i++) {
        a = *(codeImg + i) & MASK;                 /* instruction: bytes 0-8; mask:   00000000 00000000 00000000 11111111 */
        b = (*(codeImg + i) & (MASK << 8)) >> 8;   /* instruction: bytes 9-16; mask:  00000000 00000000 11111111 00000000 */
        c = (*(codeImg + i) & (MASK << 16)) >> 16; /* instruction: bytes 17-24; mask: 00000000 11111111 00000000 00000000 */
        d = (*(codeImg + i) & (MASK << 24)) >> 24; /* instruction: bytes 25-32; mask: 11111111 00000000 00000000 00000000 */

        fprintf(fp, "\n%04d %02x %02x %02x %02x", IC, a, b, c, d);
        IC += 4;
    }

    /* print data image */
    /* fills data array with data for printing, each cell is 4 bits for easier printing */
    i = 0;
    while (dataImg->next != NULL) {
        switch (dataImg->flag) {
            case asci:
            case byte:
                *(data + i) = dataImg->byte;
                break;
            case half_word:
                *(data + i++) = dataImg->half_word;
                *(data + i) = (dataImg->half_word) >> 8;
                break;
            case word:
                *(data + i++) = dataImg->word;
                *(data + i++) = (dataImg->word) >> 8;
                *(data + i++) = (dataImg->word) >> 16;
                *(data + i) = (dataImg->word) >> 24;
                break;
            default:
                break;
        }
        dataImg = dataImg->next;
        i++;
    }

    for (i = 0; i < DCF; i += 4) {
        fprintf(fp, "\n%04d ", ICF + j);
        fprintf(fp, "%02x %02x %02x %02x", data[i], data[i + 1], data[i + 2], data[i + 3]);
        j += 4;
    }
    free(data);
}

void printExt() {}

void printEnt(FILE* fp, sym_t* symbol) {
    while (symbol != NULL) {
        if (strstr(symbol->attribute, "entry")) {
            fprintf(fp, "%s %04d", symbol->name, symbol->address);
        }
        symbol = symbol->next;
    }
}
