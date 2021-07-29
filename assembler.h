#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#define NUM_OF_FUNC 27
#define NUM_OF_DIR 6
#define NUM_OF_REG 32
#define MAX_LINE_LENGTH 81 /* Limit is 80 + Null terminator*/
#define LABEL_MAX_LENGTH 31

enum {
    false = 0,
    true = 1,
    r_type = 2,
    i_type = 3,
    j_type = 4
};

typedef struct symbol {
    char *name;
    char *attribute;
    unsigned int address;
    struct symbol *next;
} sym_t;

typedef struct REG {
    unsigned int val : 32;
} reg_t;

typedef struct REG *reg_ptr;

reg_ptr regArray[NUM_OF_REG];

unsigned int r_binary_instruction();
unsigned int i_binary_instruction();
unsigned int j_binary_instruction();

#endif