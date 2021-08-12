
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

#define MASK_1_BTYE 0xFFFF /* ..00 0000 1111 1111 1111 1111 */

/* This function get the numeric value of a binary number */
int getNumericValue(int binaryNumber) {
    int tempBinary, tempRes;
    int decimalNumber = 0;
    int base = 1;

    tempBinary = binaryNumber;

    while (tempBinary > 0) {
        tempRes = tempBinary % 10;
        decimalNumber = decimalNumber + tempRes * base;
        tempBinary = tempBinary / 10;
        base = base * 2;
    }
    return tempRes;
}

/* This function get the numeric value of a binary number using the Two's complement method */
/*
int getNumericValueCompTwo(int binaryNumber, int significantBits)
{
    int base = pow(2, significantBits - 1);
    int decimalValue = 0;
    int cloneBinary = binaryNumber;
    int i;

    for (i = 0; i < significantBits; ++i)
    {
        if (i == 0 && binary[i] != '0')
        {
            decimalValue = base * -1;
        }
        else
        {
            decimalValue += (binary[i] - '0') * base;
        }
        base /= 2;
    }

    return decimalValue;
}
*/

void freeDataImg(dir_t *data) {
    while (data != NULL) {
        dir_t *temp = data;
        data = data->next;
        free(temp);
    }
}

void freeMemory(flags *flag, sym_t *symbol, dir_t *data_img, FILE *fp) {
    fclose(fp);
    freeDataImg(data_img);
    freeSymbolTable(symbol);
    free(flag);
}

void calcDistance(unsigned int *instruction_address, unsigned int label_address) {
    int dist;
    unsigned int curr_address = *instruction_address & MASK_1_BTYE; /* get the address of the instruction itself (rightmost 16 bits) */
    dist = label_address - curr_address;                            /* new address to be entered to code image = the distance between the two */
    dist = dist & MASK_1_BTYE;                                      /* get rightmost 26 bits of new address */
    *instruction_address = *instruction_address & ~MASK_1_BTYE;     /* turn off bit 0-16 of original machine code */
    *instruction_address = *instruction_address | dist;
}