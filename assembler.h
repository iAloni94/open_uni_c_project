#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

typedef struct flag {
    char label;
    char params;
    char stop;
} flags;

typedef struct REG {
    unsigned int : 32;
} reg_t;

typedef struct REG *reg_ptr;

reg_ptr *regArray[32];
unsigned int DC = 0;
unsigned int IC = 100;

unsigned int r_binary_instruction();
unsigned int i_binary_instruction();
unsigned int j_binary_instruction();

#endif