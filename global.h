/*
* This file contains ALL the function declaration from all c files and also includes the main header files.
*/

#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <stdio.h>

#include "assembler.h"
#include "directive.h"
#include "instruction.h"
#include "label.h"
#include "utils.h"

void firstPass(unsigned int *, unsigned int *, unsigned int *, unsigned int *, unsigned int *, dir_t *, sym_t *, flags *, FILE *);
void secondPass(unsigned int, unsigned int, unsigned int *, sym_t *, flags *, FILE *);

/* to_binary.c */
unsigned int r_binary_instruction(R *);
unsigned int i_binary_instruction(I *);
unsigned int j_binary_instruction(J *);

/* param.c */
R *check_r_param(int, node_t *, R *, flags *);
I *check_i_param(int, node_t *, I *, flags *);
J *check_j_param(int, node_t *, J *, flags *, sym_t *);
char getReg(node_t *, flags *);

/* utils.c */
void freeMemory(flags *, sym_t *, dir_t *, FILE *);
void freeDataImg(dir_t *);

/* input.c */
node_t *getLine(FILE *, flags *);
node_t *initList();
node_t *addNode();
void freeInputList(node_t *);

/* write_files.c */
FILE *createFile(char *, char *);
void printObj(FILE *, unsigned int *, dir_t *, unsigned int, unsigned int);
void printExt();
void printEnt();
void writeFiles(char *, unsigned int *, dir_t *, sym_t *, flags *, unsigned int, unsigned int);

/* label.c */
void freeSymbolTable(sym_t *);
unsigned int getSymbolAddress(char *, sym_t *);
bool isLabel(node_t *, flags *, sym_t *);
bool isAlphaNumeric(char *);
bool isReserved(char *, flags *);
bool isDeclared(char *, sym_t *, flags *);
void insertLabel(sym_t *, node_t *, flags *, unsigned int, unsigned int);
void updateSymbolAddress(sym_t *, unsigned int);

/* directive.c */
bool checkNum(node_t *, flags *);
char *checkStr(node_t *, flags *);
dir_t *save_byte(node_t *, dir_t *, unsigned int *, flags *);
dir_t *save_half_word(node_t *, dir_t *, unsigned int *, flags *);
dir_t *save_word(node_t *, dir_t *, unsigned int *, flags *);
dir_t *save_str(node_t *, dir_t *, unsigned int *, flags *);
void ext_handler(sym_t *, node_t *, flags *, unsigned int, unsigned int);
void ent_handler(sym_t *, node_t *, flags *);
#endif