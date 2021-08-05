#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

char *savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

node_t *initList()
{
    node_t *head = malloc(sizeof(node_t));
    head->val = calloc(1, 100);
    head->next = NULL;
    return head;
}

node_t *addNode()
{
    node_t *newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, 100);
    newNode->next = NULL;
    return newNode;
}

void freeList(node_t *node)
{
    while (node != NULL)
    {
        node_t *currNode = node;
        node = node->next;
        free(currNode);
    }
    free(node);
}

char isAlphaNumeric(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (!IS_LETTER(*(str + i)) && !IS_NUM(*(str + i)))
            return false;
    }
    return true;
}

char isReserved(char *str)
{
    int i;
    for (i = 0; i < 33; i++)
    { /* 33 is number of saved words*/
        if (strcmp(savedWords[i], str) == 0)
            return true;
    }
    return false;
}

void isDeclared(char *str, sym_t *symbol, flags *flag)
{
    while (symbol->name != NULL)
    {
        if (strcmp(str, symbol->name) == 0)
        {
            printf("\nLine: %d - Label was already declared", flag->line);
            flag->firstPass = false;
        }
        symbol = symbol->next;
    }
}

/* This function get the numeric value of a binary number */
int getNumericValue(int binaryNumber)
{
    int tempBinary, tempRes;
    int decimalNumber = 0;
    int base = 1;

    tempBinary = binaryNumber;

    while (tempBinary > 0)
    {
        tempRes = tempBinary % 10;
        decimalNumber = decimalNumber + tempRes * base;
        tempBinary = tempBinary / 10;
        base = base * 2;
    }
    return tempRes;
}

/* This function get the numeric value of a binary number using the Two's complement method */
// int getNumericValueCompTwo(int binaryNumber, int significantBits)
// {
//     int base = pow(2, significantBits - 1);
//     int decimalValue = 0;
//     int cloneBinary = binaryNumber;
//     int i;

//     for (i = 0; i < significantBits; ++i)
//     {
//         if (i == 0 && binary[i] != '0')
//         {
//             decimalValue = base * -1;
//         }
//         else
//         {
//             decimalValue += (binary[i] - '0') * base;
//         }
//         base /= 2;
//     }

//     return decimalValue;
// }
