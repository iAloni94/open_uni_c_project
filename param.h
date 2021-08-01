#ifndef __PARAM_H
#define __PARAM_H

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
    unsigned int immed : 16;
} I;

typedef struct J {
    unsigned int opcode : 6;
    unsigned int reg : 1;
    unsigned int address : 25;
} J;

R *check_r_param();
I *check_i_param();
J *check_j_param();

#endif