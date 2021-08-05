#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* 00000000 01100101 01001000 01000000 */
/* 01111100 00000000 00000000 00000000 */
#define MASK_1 255        /* 00000000 00000000 00000000 11111111 */
#define MASK_2 65280      /* 00000000 00000000 11111111 00000000 */
#define MASK_3 16711680   /* 00000000 11111111 00000000 00000000 */
#define MASK_4 4278190080 /* 11111111 00000000 00000000 00000000 */

void printObj(FILE *fp, unsigned int *codeImg, unsigned int *codeAddress, unsigned int ICF, unsigned int DCF) {
    unsigned int i = 0;
    unsigned int a, b, c, d;

    fprintf(fp, "%d\t%d\n", ICF-100, DCF);

    for (i = 0; i < 1000; i++) {
        a = *(codeImg + i) & MASK_1;
        b = (*(codeImg + i) & MASK_2) >> 8;
        c = (*(codeImg + i) & MASK_3) >> 16;
        d = (*(codeImg + i) & MASK_4) >> 24;

        fprintf(fp, "%04d %02x %02x %02x %02x\n",  *(codeAddress + i), a, b, c, d);
    }
}
