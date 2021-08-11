#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

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
        free(currNode);
    }
    free(node);
}

unsigned int getSymbolAddress(char *name, sym_t *symbol) {
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(name, symbol->name)) {
            return symbol->address;
        }
        symbol = symbol->next;
    }
    return false;
}

char isLabel(node_t *input, flags *flag, sym_t *symbol) {
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

char isAlphaNumeric(char *str) {
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

char isReserved(char *str, flags *flag) { /* checks if label is a reseved word */
    int i;
    for (i = 0; i < 33; i++) { /* 33 is number of reserved words*/
        if (!strcmp(savedWords[i], str)) {
            flag->firstPass = false;
            return true;
        }
    }
    return false;
}

char isDeclared(char *str, sym_t *symbol, flags *flag) { /* this functions check if a label was already decalred*/
    while (symbol != NULL && symbol->name != NULL) {
        if (!strcmp(str, symbol->name)) {
            if (!flag->entry) {
                printf("\nLine: %d - Label name <%s> already in use", flag->line, str);
                flag->firstPass = false;
                return true; /* label was declared */
            } else if (!strcmp(symbol->attribute, "external")) {
                printf("\nLine: %d - Label was already declared as external", flag->line);
                flag->secondPass = false;
                return false;
            }
            return true;
        }
        symbol = symbol->next;
    }
    if (flag->entry) {
        printf("\nLine: %d - Label was not declared", flag->line);
        flag->entry = false;
        flag->secondPass = false;
    }

    return false;
}

void insertLabel(sym_t *symbol, node_t *head, flags *flag, unsigned int IC, unsigned int DC) {
    if (symbol == NULL) {
        symbol = calloc(sizeof(sym_t), 1);
    } else if (symbol != NULL && symbol->name != NULL) {
        while (symbol->next != NULL) symbol = symbol->next;
        symbol->next = calloc(sizeof(sym_t), 1);
        symbol = symbol->next;
    }
    symbol->name = calloc(sizeof(char), strlen(head->val));
    memcpy(symbol->name, head->val, strlen(head->val));
    if (flag->direction) {
        if (flag->external) {
            symbol->attribute = "external";
        } else {
            symbol->attribute = "data";
        }
        symbol->address = DC;
        flag->line += 1;
    } else {
        symbol->address = IC;
        symbol->attribute = "code";
    }
    flag->label = false;
}

void updateSymbolAddress(sym_t *symbol, unsigned int ICF) {
    while (symbol != NULL) {
        if (!strcmp(symbol->attribute, "data")) {
            symbol->address += ICF;
        }
        symbol = symbol->next;
    }
}