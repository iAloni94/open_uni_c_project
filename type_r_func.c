/* 
* R type Instructions.
* Math and Logic Instructions: add, sub, and, or, nor.
* Copy Instructions: move, mvhi, mvlo.
* Each Instruction has 3 operands and 3 registers
* The Instructions is being done between the "rs" and the "rt" registers, and the resualt is being saved in the "rd" register.
* All of the Instructions have the same code, opcode = 0 (include nothing).
* Each Instruction has a uniqe name field, the "funct" field, which is uniqe for each Instruction.
*/


#include "global.h"

void add_func(R *instruction)
{
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs + rt;
    regArray[instruction->rd]->val = rd;
}

void sub_func(R *instruction)
{
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs - rt;
    regArray[instruction->rd]->val = rd;
}
void and_func(R *instruction)
{
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs & rt;
    regArray[instruction->rd]->val = rd;
}
void or_func(R *instruction)
{
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = (rs | rt);
    regArray[instruction->rd]->val = rd;
}
void nor_func(R *instruction)
{
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = ~(rs | rt);
    regArray[instruction->rd]->val = rd;
}

/* This function copies the value from rs to rd */

void move_func(R *instruction)
{
    regArray[instruction->rd]->val = (regArray[instruction->rs]->val);
}

/* This function copies the bits 16-31 from rs to rd */

void mvhi_func(R *instruction)
{
    /* copy bits 16-31 from rs to rd */
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rd = (regArray[instruction->rd]->val);
    for (i = 31; i > 15; i--)
    {
        char bit = CHECK_BIT(rs, i);
        CHANGE_BIT(rd, bit, i);
    }
    regArray[instruction->rd]->val = rd;
}

/* This function copies the bits 0-15 from rs to rd */

void mvlo_func(R *instruction)
{
    /* copy bits 0-15 from rs to rd */
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rd = (regArray[instruction->rd]->val);
    for (i = 0; i < 16; i++)
    {
        char bit = CHECK_BIT(rs, i);
        CHANGE_BIT(rd, bit, i);
    }
    regArray[instruction->rd]->val = rd;
}