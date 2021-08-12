#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

#define RES_WORD_NUM 33

char *savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

void freeSymbolTable(sym_t *node) {
    sym_t *currNode = node;
    while (node != NULL) {
        currNode = node;
        node = node->next;
        free(currNode->name);
        free(currNode->attribute);
        free(currNode);
    }
    free(node);
}

/* return the address of @name */
unsigned int getSymbolAddress(char *name, sym_t *symbol) {
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(name, symbol->name)) {
            return symbol->address;
        }
        symbol = symbol->next;
    }
    return false;
}

/*
* This function is used to validate a label.
* It check that all characters are alphanumeric, that its length does not exceeds the limit and that no reserved word are used as labels.
* it expects a colon to be present at the end of the string, so if there isnt one (when a label is used as an operand), one must be added.
*/

bool isLabel(node_t *input, flags *flag, sym_t *symbol) {
    if (strchr(input->val, ':') != NULL) {
        input->val[strlen(input->val) - 1] = '\0';
        if ((strlen(input->val) <= LABEL_MAX_LENGTH) &&
            (isAlphaNumeric(input->val)) &&
            (!isReserved(input->val, flag)))
            return true;
        else {
            flag->firstPass = false;
            printf("\nLine: %d - Illegal label name", flag->line);
        }
    }
    return false;
}

bool isAlphaNumeric(char *str) {
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

/* checks if label is a reseved word */
bool isReserved(char *str, flags *flag) {
    int i;
    for (i = 0; i < RES_WORD_NUM; i++) {
        if (!strcmp(savedWords[i], str)) {
            flag->firstPass = false;
            return true;
        }
    }
    return false;
}

/* this functions check if a label was already decalred */
bool isDeclared(char *str, sym_t *symbol, flags *flag) {
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(str, symbol->name)) {
            if (!flag->entry) { /* first pass check */
                printf("\nLine: %d - Label name \"%s\" was already declared", flag->line, str);
                flag->firstPass = false;
                return true;                                     /* label was declared */
            } else if (!strcmp(symbol->attribute, "external")) { /* if entry, check if external */
                printf("\nLine: %d - Label was already declared as external", flag->line);
                flag->secondPass = false;
                return false;
            }
            return true;
        }
        symbol = symbol->next;
    }
    if (flag->entry) { /* if entry and label was not decalred */
        printf("\nLine: %d - Label was not declared", flag->line);
        flag->entry = false;
        flag->secondPass = false;
    }

    return false;
}

/* insert new label to symbol table */
void insertLabel(sym_t *symbol, node_t *head, flags *flag, unsigned int IC, unsigned int DC) {
    if (symbol != NULL && symbol->name != NULL) { /* if first node */
        while (symbol->next != NULL) symbol = symbol->next;
        symbol->next = calloc(sizeof(sym_t), 1);
        symbol = symbol->next;
    }
    symbol->name = calloc(sizeof(char), strlen(head->val));
    symbol->attribute = calloc(sizeof(char), 25);
    memcpy(symbol->name, head->val, strlen(head->val));
    if (flag->direction) {
        if (flag->isExt) {
            memcpy(symbol->attribute, "external", strlen("external"));
        } else {
            memcpy(symbol->attribute, "data", strlen("data"));
        }
        symbol->address = DC;
    } else {
        symbol->address = IC;
        memcpy(symbol->attribute, "code", strlen("code"));
    }
    flag->label = false;
}

/* Updates each symbol address after first pass */
void updateSymbolAddress(sym_t *symbol, unsigned int ICF) {
    while (symbol != NULL) {
        if (!strcmp(symbol->attribute, "data")) {
            symbol->address += ICF;
        }
        symbol = symbol->next;
    }
}