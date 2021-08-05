#ifndef __UTILS_H
#define __UTILS_H

#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false     /* 48 = '0', 57 = '9' */

#include <stdio.h>

enum {
    false = 0,
    true = 1,
};

typedef enum functions {
    add = 0,
    sub = 1,
    and = 2,
    or = 3,
    nor = 4,
    move = 5,
    mvhi = 6,
    mvlo = 7,
    addi = 8,
    subi = 9,
    andi = 10,
    ori = 11,
    nori = 12,
    bne = 13,
    beq = 14,
    blt = 15,
    bgt = 16,
    lb = 17,
    sb = 18,
    lw = 19,
    sw = 20,
    lh = 21,
    sh = 22,
    jmp = 23,
    la = 24,
    call = 25,
    stop = 26
} functions;

typedef enum function_opcode {
    mv_opcode = 1,
    addi_opcode = 10,
    subi_opcode = 11,
    andi_opcode = 12,
    ori_opcode = 13,
    nori_opcode = 14,
    bne_opcode = 15,
    beq_opcode = 16,
    blt_opcode = 17,
    bgt_opcode = 18,
    lb_opcode = 19,
    sb_opcode = 20,
    lw_opcode = 21,
    sw_opcode = 22,
    lh_opcode = 23,
    sh_opcode = 24,
    jmp_opcode = 30,
    la_opcode = 31,
    call_opcode = 32,
    stop_opcode = 63
};

typedef struct flag { /* this struct contains all the differnt flags and indicators for the assembly operation */
    char label;
    char firstPass;
    char direction;
    char lastLine;
    char external;
    char entry;
    unsigned int line;
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;

FILE *createFile();
node_t *initList();
node_t *addNode();

unsigned int getSymbolAddress();

char isAlphaNumeric();
char isReserved();
char isDeclared();
void freeList();

void freeSymbolTable();
void freeInputList();

/* these function are in input.c */
node_t *getLine();
char isLabel();
char isColon();

#endif