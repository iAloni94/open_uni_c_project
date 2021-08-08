#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"

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

void printObj(FILE* fp, unsigned int* codeImg, char dataImg[], unsigned int ICF, unsigned int DCF) {
    unsigned int i = 0;
    unsigned int a, b, c, d;
    unsigned int IC = 100;

    fprintf(fp, "%d\t%d\n", ICF - 100, DCF);

    /*  print code image*/
    /* shift mask to desired byte location and then shift the result to the rightmost byte for printing */
    for (i = 0; *(codeImg + i) != 0; i++) {
        a = *(codeImg + i) & MASK;                 /* instruction: bytes 0-8; mask:   00000000 00000000 00000000 11111111 */
        b = (*(codeImg + i) & (MASK << 8)) >> 8;   /* instruction: bytes 9-16; mask:  00000000 00000000 11111111 00000000 */
        c = (*(codeImg + i) & (MASK << 16)) >> 16; /* instruction: bytes 17-24; mask: 00000000 11111111 00000000 00000000 */
        d = (*(codeImg + i) & (MASK << 24)) >> 24; /* instruction: bytes 25-32; mask: 11111111 00000000 00000000 00000000 */

        fprintf(fp, "%04d %02x %02x %02x %02x\n", IC, a, b, c, d);
        IC += 4;
    }

    /* print data image: */
}

void printExt() {}
void printEnt() {}
