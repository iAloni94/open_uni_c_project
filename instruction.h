/*
* This file contains instructions structs
*/

#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

/* this enum is for code readability and is based on the table from the maman instructions */
typedef enum functions {
    /* logical instructions */
    add = 0,
    sub = 1,
    and = 2,
    or = 3,
    nor = 4,
    /* copy instructions */
    move = 5,
    mvhi = 6,
    mvlo = 7,
    /* logical immediate instructions */
    addi = 8,
    subi = 9,
    andi = 10,
    ori = 11,
    nori = 12,
    /* conditional branch instructions */
    beq = 13,
    bne = 14,
    blt = 15,
    bgt = 16,
    /* load and save instructions */
    lb = 17,
    sb = 18,
    lw = 19,
    sw = 20,
    lh = 21,
    sh = 22,
    /* assembly guidance instructions */
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
} opcode;

typedef struct R {
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    unsigned int rd : 5;
    unsigned int funct : 5;
    unsigned int notused : 6;
} R;

typedef struct I {
    unsigned int opcode : 6;
    unsigned int rs : 5;
    unsigned int rt : 5;
    int immed : 16; /* this is signed because it can accept negative integers */
} I;

typedef struct J {
    unsigned int opcode : 6;
    unsigned int reg : 1;
    unsigned int address : 25;
} J;

#endif