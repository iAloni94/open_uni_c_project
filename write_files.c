#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"

#define MASK 0xFF /* 00000000 00000000 00000000 11111111 */
#define KEEP_ONLY_24_LSB(value) ((value)&0xFFFFFF)

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
    unsigned int i = 0;
    unsigned int a, b, c, d;
    unsigned int IC = 100;
    long val[1200];

    fprintf(fp, "%d\t%d", ICF - 100, DCF);

    /*  print code image*/
    /* shift mask to desired byte location and then shift the result to the rightmost byte for printing */
    for (i = 0; *(codeImg + i) != 0; i++) {
        a = *(codeImg + i) & MASK;                 /* instruction: bytes 0-8; mask:   00000000 00000000 00000000 11111111 */
        b = (*(codeImg + i) & (MASK << 8)) >> 8;   /* instruction: bytes 9-16; mask:  00000000 00000000 11111111 00000000 */
        c = (*(codeImg + i) & (MASK << 16)) >> 16; /* instruction: bytes 17-24; mask: 00000000 11111111 00000000 00000000 */
        d = (*(codeImg + i) & (MASK << 24)) >> 24; /* instruction: bytes 25-32; mask: 11111111 00000000 00000000 00000000 */

        fprintf(fp, "\n%04d %02x %02x %02x %02x", IC, a, b, c, d);
        IC += 4;
    }

    /* print data image */
    unsigned char data[1000] = {0};
    i = 0;
    while (dataImg->next != NULL) {
        switch (dataImg->flag) {
            case asci:
            case byte:
                data[i] = dataImg->byte;
                break;
            case half_word:
                data[i++] = dataImg->half_word;
                data[i] = (dataImg->half_word) >> 8;
                break;
            case word:
                /* 31 = 0000 0000 0000 0000 0000 0000 0001 1111  */
                data[i++] = dataImg->word;
                data[i++] = (dataImg->word) >> 8;
                data[i++] = (dataImg->word) >> 16;
                data[i] = (dataImg->word) >> 24;
                break;
            default:
                break;
        }
        dataImg = dataImg->next;
        i++;
    }
    int j = 0;
    for (i = 0; i < 100; i += 4) {
        fprintf(fp, "\n%04d ", ICF + j);
        fprintf(fp, " %02x %02x %02x %02x", data[i], data[i + 1], data[i + 2], data[i + 3]);
        j += 4;
    }
}

void printExt() {}
void printEnt() {}
