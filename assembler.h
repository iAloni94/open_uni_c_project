/*
* This file contains the symbol table and register structs
*/

#ifndef __ASSEMBLER_H 
#define __ASSEMBLER_H

#define NUM_OF_FUNC 27
#define NUM_OF_DIR 6
#define NUM_OF_REG 32
#define MAX_LINE_LENGTH 81 /* Limit is 80 + Null terminator*/
#define LABEL_MAX_LENGTH 31
#define FILE_EXT ".as"
#define MEM_SIZE 33554432

typedef struct symbol {
    char *name;
    char *attribute; /* code, data, external, entry */
    unsigned int address;
    struct symbol *next;
} sym_t;

#endif