#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MASK_1 0xFF        /* 00000000 00000000 00000000 11111111 */
#define MASK_2 0xFF00      /* 00000000 00000000 11111111 00000000 */
#define MASK_3 0xFF0000   /* 00000000 11111111 00000000 00000000 */
#define MASK_4 0xFF000000 /* 11111111 00000000 00000000 00000000 */

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

void printObj(FILE* fp, unsigned int* codeImg, unsigned int* codeAddress, unsigned int ICF, unsigned int DCF) {
    unsigned int i = 0;
    unsigned int a, b, c, d;

    fprintf(fp, "%d\t%d\n", ICF - 100, DCF);

    for (i = 0; *(codeImg + i) != 0; i++) {
        a = *(codeImg + i) & MASK_1;
        b = (*(codeImg + i) & MASK_2) >> 8;
        c = (*(codeImg + i) & MASK_3) >> 16;
        d = (*(codeImg + i) & MASK_4) >> 24;

        fprintf(fp, "%04d %02x %02x %02x %02x\n", *(codeAddress + i), a, b, c, d);
    }
}

void printExt(){}
void printEnt(){}
