#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"

char* savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

node_t* initList() {
    node_t* head = malloc(sizeof(node_t));
    head->val = calloc(1, 100);
    head->next = NULL;
    return head;
}

node_t* addNode() {
    node_t* newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, 100);
    newNode->next = NULL;
    return newNode;
}

void freeInputList(node_t* node) {
    node_t* currNode;
    while (node != NULL) {
        currNode = node;
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

char isAlphaNumeric(char* str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (IS_LETTER(*(str + i)) || IS_NUM(*(str + i))) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

char isReserved(char* str, flags* flag) { /* checks if label is a reseved word */
    int i;
    for (i = 0; i < 33; i++) { /* 33 is number of reserved words*/
        if (!strcmp(savedWords[i], str)) {
            flag->firstPass = false;
            return true;
        }
    }
    return false;
}

char isDeclared(char* str, sym_t* symbol, flags* flag) { /* this functions check if a label was already decalred*/
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(str, symbol->name)) {
            printf("\nLine: %d - Label name already in use", flag->line);
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

FILE* createFile(char* fname, char* extention) {
    int i;
    char ext_diff = strlen(fname) - 3;
    char* fname_no_extention = calloc(sizeof(char), ext_diff); /* length of ".as" = 3 */
    FILE* output_file;

    for (i = 0; i < (ext_diff); i++) {
        *(fname_no_extention + i) = *(fname + i);
    }
    /* Opening file for output */
    output_file = fopen(strcat(fname_no_extention, extention), "w");
    free(fname_no_extention);
    return output_file;
}
