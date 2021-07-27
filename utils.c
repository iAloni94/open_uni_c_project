#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"



char* savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

char isAlphaNumeric(char* str) {
    int i;
    for (i = 0; i < strlen(str); i++) {
        if (!IS_LETTER(*(str + i)) && !IS_NUM(*(str + i))) return false;
    }
    return true;
}

char isReserved(char* str) {
    int i;
    for (i = 0; i < 33; i++) { /* 33 is number of saved words*/
        if (strcmp(savedWords[i], str) == 0) return true;
        return false;
    }
}

void isDeclared(char* str, sym_t* symbol, flags* flag) {
    while (symbol != NULL) {
        if (strcmp(str, symbol->name) == 0) {
            printf("\nLine: %d - Label was already declared", flag->line);
            flag->firstPass = false;
        }
        symbol = symbol->next;
    }
}


void freeList(node_t *node) {
    while (node != NULL) {
        node_t *currNode = node;
        node = node->next;
        free(currNode);
    }
    free(node);
}