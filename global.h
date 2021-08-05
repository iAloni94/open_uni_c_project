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

/* type_r_param.c */
void add_func(R *);
void sub_func(R *);
void and_func(R *);
void or_func(R *);
void nor_func(R *);
void move_func(R *);
void mvhi_func(R *);
void mvlo_func(R *);

/* type_i_param.c */
void addi_func(I *);
void subi_func(I *);
void andi_func(I *);
void ori_func(I *);
void nori_func(I *);
void bne_func(I *);
void beq_func(I *);
void blt_func(I *);
void bgt_func(I *);
void lb_func(I *);
void sb_func(I *);
void lw_func(I *);
void sw_func(I *);
void lh_func(I *);
void sh_func(I *);

/* type_j_param.c */
void jmp_func(J *);
void la_func(J *);
void call_func(J *, unsigned int);
void stop_func(J *);

/* param.c */
R *check_r_param(int, node_t *, R *, flags *);
I *check_i_param(int, node_t *, I *, flags *);
J *check_j_param(int, node_t *, J *, flags *, sym_t *);

/* utils.c */
node_t *initList();
node_t *addNode();
unsigned int getSymbolAddress(char *, sym_t *);
char isAlphaNumeric(char *);
char isReserved(char *, flags *);
char isDeclared(char *, sym_t *, flags *);
void freeSymbolTable(sym_t *);
void freeInputList(node_t *);

/* input.c */
node_t *getLine(FILE *, flags *);
char isLabel(node_t *, flags *, sym_t *);
char isColon(char *, flags *);

/* write_files.c */
FILE *createFile(char *, char *);
void printObj(FILE *, unsigned int *, unsigned int *, unsigned int, unsigned int);

#endif