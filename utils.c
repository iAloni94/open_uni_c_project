
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

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