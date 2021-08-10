
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
node_t *initList() {
    node_t *head = malloc(sizeof(node_t));
    head->val = calloc(1, 100);
    head->next = NULL;
    return head;
}

node_t *addNode() {
    node_t *newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, 100);
    newNode->next = NULL;
    return newNode;
}

void freeInputList(node_t *node) {
    node_t *currNode;
    while (node != NULL) {
        currNode = node;
        node = node->next;
        free(currNode->val);
        free(currNode);
    }
}

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

void freeDataImg(dir_t *data){
    while(data!=NULL){
        dir_t *temp = data;
        data = data->next;
        free(temp);
    }
}

void freeMemory(flags *flag, sym_t *symbol, dir_t *data_img, FILE *fp, FILE *fp_obj, FILE *fp_ext, FILE *fp_ent) {
    if (flag->external) fclose(fp_ext);
    if (flag->entry) fclose(fp_ent);
    if (flag->secondPass) fclose(fp_obj);
    fclose(fp);
    freeDataImg(data_img);
    freeSymbolTable(symbol);
    free(flag);
}