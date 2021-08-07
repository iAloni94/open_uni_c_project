/*
* This file contains ALL the function declaration from all c files and also includes the main header files.
*/

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdio.h>

#include "assembler.h"
#include "instruction.h"
#include "utils.h"

/* assembler.c */
unsigned int r_binary_instruction(R *);
unsigned int i_binary_instruction(I *);
unsigned int j_binary_instruction(J *);

/* param.c */
R *check_r_param(int, node_t *, R *, flags *);
I *check_i_param(int, node_t *, I *, flags *);
J *check_j_param(int, node_t *, J *, flags *, sym_t *);

/* utils.c */
node_t *initList();
node_t *addNode();
void freeInputList(node_t *);
void updateDataAddress(unsigned int *, unsigned int);
void freeMemory(flags *, sym_t *, FILE *, FILE *, FILE *, FILE *);

/* input.c */
node_t *getLine(FILE *, flags *);

/* write_files.c */
FILE *createFile(char *, char *);
void printObj(FILE *, unsigned int *, unsigned int *, unsigned int *, unsigned int, unsigned int);
void printExt();
void printEnt();

/* label.c */
void freeSymbolTable(sym_t *);
unsigned int getSymbolAddress(char *, sym_t *);
char isLabel(node_t *, flags *, sym_t *);
char isAlphaNumeric(char *);
char isReserved(char *, flags *);
char isDeclared(char *, sym_t *, flags *);
void insertLabel(sym_t *, node_t *, flags *, unsigned int, unsigned int);
void updateSymbolAddress(sym_t *, unsigned int);
#endif