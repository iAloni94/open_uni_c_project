#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#define NUM_OF_FUNC 27
#define NUM_OF_DIR 6
#define NUM_OF_REG 32

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


unsigned int r_binary_instruction();
unsigned int i_binary_instruction();
unsigned int j_binary_instruction();

#endif