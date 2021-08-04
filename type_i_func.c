/* 
*I type Instructions.
* Math and Logic Instructions: addi, subi, andi, ori, nori.
* Copy Instructions: move, mvhi, mvlo.
* Each Instruction has 3 operands and 3 registers
* The Instructions is being done between the "rs" and the "rt" registers, and the resualt is being saved in the "rd" register.
* All of the Instructions have the same code, opcode = 0 (include nothing).
* Each Instruction has a uniqe name field, the "funct" field, which is uniqe for each Instruction.
*/

#include "assembler.h"
#include "param.h"
#include "func.h"

/* 
*This function adds the value of the immediate value ('inned')
* to the value in register ('rs') and save the action resault in 'rt'
*/
void addi_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = immed + rs_val;
    (regArray[instruction->rt]->val) = res;
}

/*  This function subtrac the value of the immediate value from the value in 'rs' */

void subi_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = immed - rs_val;
    (regArray[instruction->rt]->val) = res;
}

/*  This function simulate the "and" (&) action between the immediate value and the value in 'rs' */

void andi_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = immed & rs_val;
    (regArray[instruction->rt]->val) = res;
}

/*  This function simulate the "or" (||) action between the immediate value and the value in 'rs' */

void ori_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = immed | rs_val;
    (regArray[instruction->rt]->val) = res;
}

/*  This function simulate the "nor" (!(a|| b)) action between the immediate value and the value in 'rs' */

void nori_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = ~(immed | rs_val);
    (regArray[instruction->rt]->val) = res;
}

/*  This function simulate the "and" (&) action between the immediate value and the value in 'rs' */

void bne_func(I *instruction)
{
}
void beq_func(I *instruction)
{
}
void blt_func(I *instruction)
{
}
void bgt_func(I *instruction)
{
}
void lb_func(I *instruction)
{
}
void sb_func(I *instruction)
{
}
void lw_func(I *instruction)
{
}
void sw_func(I *instruction)
{
}
void lh_func(I *instruction)
{
}
void sh_func(I *instruction)
{
}