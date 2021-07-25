#ifndef __ASSEMBLER_H
#define __ASSEMBLER_H

#define NUM_OF_FUNC 27
#define NUM_OF_DIR 6
#define NUM_OF_REG 32

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)       \
    (byte & 0x80 ? '1' : '0'),     \
        (byte & 0x40 ? '1' : '0'), \
        (byte & 0x20 ? '1' : '0'), \
        (byte & 0x10 ? '1' : '0'), \
        (byte & 0x08 ? '1' : '0'), \
        (byte & 0x04 ? '1' : '0'), \
        (byte & 0x02 ? '1' : '0'), \
        (byte & 0x01 ? '1' : '0')

typedef struct flag {
    char label;
    char params;
    char stop;
    char error;
    unsigned int line;
    char pass;
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