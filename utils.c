#include "utils.h"

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

<<<<<<< HEAD
void freeList(node_t *node)
{
    while (node != NULL)
    {
        node_t *currNode = node;
=======
void freeInputList(node_t* node) {
    node_t* currNode;
    while (node != NULL) {
        currNode = node;
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
        node = node->next;
        free(currNode->val);
        free(currNode);
    }
}

void freeSymbolTable(sym_t* node) {
    sym_t* currNode = node;
    while (node != NULL) {
        currNode = node;
        node = node->next;
        free(currNode->name);
        free(currNode);
    }
    free(node);
}

char isAlphaNumeric(char *str)
{
    int i;
<<<<<<< HEAD
    for (i = 0; i < strlen(str); i++)
    {
        if (!IS_LETTER(*(str + i)) && !IS_NUM(*(str + i)))
            return false;
=======
    for (i = 0; i < strlen(str); i++) {
        if (IS_LETTER(*(str + i)) || IS_NUM(*(str + i))) {
            continue;
        } else {
            return false;
        }
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
    }
    return true;
}

<<<<<<< HEAD
char isReserved(char *str)
{
    int i;
    for (i = 0; i < 33; i++)
    { /* 33 is number of saved words*/
        if (strcmp(savedWords[i], str) == 0)
            return true;
=======
char isReserved(char* str, flags* flag) { /* checks if label is a reseved word */
    int i;
    for (i = 0; i < 33; i++) { /* 33 is number of reserved words*/
        if (!strcmp(savedWords[i], str)) {
            flag->firstPass = false;
            return true;
        }
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
    }
    return false;
}

<<<<<<< HEAD
void isDeclared(char *str, sym_t *symbol, flags *flag)
{
    while (symbol->name != NULL)
    {
        if (strcmp(str, symbol->name) == 0)
        {
            printf("\nLine: %d - Label was already declared", flag->line);
=======
char isDeclared(char* str, sym_t* symbol, flags* flag) { /* this functions check if a label was already decalred*/
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(str, symbol->name)) {
            printf("\nLine: %d - Label name already in use", flag->line);
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
            flag->firstPass = false;
            return true; /* label was declared */
        }
        symbol = symbol->next;
    }
    return false;
}

unsigned int getSymbolAddress(char* name, sym_t* symbol) {
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(name, symbol->name)) {
            return symbol->address;
        }
        symbol = symbol->next;
    }
    return false;
}
<<<<<<< HEAD

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
=======
 
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
