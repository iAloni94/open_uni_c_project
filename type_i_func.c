<<<<<<< HEAD
/* 
*I type Instructions.
* Math and Logic Instructions: addi, subi, andi, ori, nori.
* Conditional Direction Instructions: beq, bne, blt, bgt.
* Load and storage Instructions: lb, sb, lw, sw, lh, sh.
* Each Instruction has 1 immediate valu named "immed". It is a immediate value.
* The Instructions is being done between the "rs" and the "immed" registers, and the resualt is being saved in the "rt" register.
* Each Instruction has a uniqe name field, the "funct" field, which is uniqe for each Instruction.
*/

#include "assembler.h"
#include "param.h"
#include "func.h"
=======
#include "global.h"
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9

/* Here are 4 Math and Logic Instructions: addi, subi, andi, ori, nori. */

/* This function adds the value of the immediate value ('immed') to the value in register ('rs') and save the resault in 'rt' */

void addi_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int res = immed + rs_val;
    (regArray[instruction->rt]->val) = res;
}

<<<<<<< HEAD
/*  This function subtrac the value of the immediate value from the value in 'rs' */

void subi_func(I *instruction)
{
=======
void subi_func(I* instruction) { 
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
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

/*
* Here are 4 Conditional Direction Instructions:  beq, bne, blt, bgt.
* In each function, if the codition is true,  the "jump" is being executed and the function in the "label" is being executed.
* else, the following instruction is being executed.
*/

/*  This function checks if the value in the 'rs' register is the same as the value in the 'rt' register*/

void bne_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int rt_index = instruction->rt;
    unsigned int rt_val = (regArray[rt_index]->val);
    unsigned int immed = instruction->immed;

    if (rs_val == rt_val)
    {
        instruction->opcode = instruction->opcode + instruction->immed;
    }
}

/*  This function checks if the value in the 'rs' register different than the value in the 'rt' register */

void beq_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int rt_index = instruction->rt;
    unsigned int rt_val = (regArray[rt_index]->val);
    unsigned int immed = instruction->immed;

    if (rs_val != rt_val)
    {
        instruction->opcode = instruction->opcode + instruction->immed;
        /* Yoav: I think we need to use here in jmp_func wdyt? */
    }
}

/*  This function checks if the value in the 'rs' register is smaller than the value in the 'rt' register */

void blt_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int rt_index = instruction->rt;
    unsigned int rt_val = (regArray[rt_index]->val);
    unsigned int immed = instruction->immed;

    if (rs_val < rt_val)
    {
        instruction->opcode = instruction->opcode + instruction->immed;
        /* Yoav: I think we need to use here in jmp_func wdyt? */
    }
}

/*  This function checks if the value in the 'rs' register is bigger than the value in the 'rt' register */

void bgt_func(I *instruction)
{
    unsigned int rs_index = instruction->rs;
    unsigned int rs_val = (regArray[rs_index]->val);
    unsigned int rt_index = instruction->rt;
    unsigned int rt_val = (regArray[rt_index]->val);
    unsigned int immed = instruction->immed;

    if (rs_val > rt_val)
    {
        instruction->opcode = instruction->opcode + instruction->immed;
        /* Yoav: I think we need to use here in jmp_func wdyt? */
    }
}

/*
* Here are 6 Load and storage Instructions: lb, sb, lw, sw, lh, sh.
* Each one of these functions reach a location in the memory that is the sum of the 'rs' register and the value in 
* the immediate value - 'immed'.
* else, the following instruction is being executed.
*/

/* */

void lb_func(I *instruction)
{
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int immed = instruction->immed;
    // int immedNumericValue =
    unsigned int memDist = rs + immed;

    for (i = 7; i > -1; i--)
    {
        // CHANGE_BIT(rd, bit, i);
    }
    // regArray[instruction->rd]->val = rd;
}

/* */

void sb_func(I *instruction)
{
}

/* */

void lw_func(I *instruction)
{
}

/* */

void sw_func(I *instruction)
{
}

/* */

void lh_func(I *instruction)
{
}
<<<<<<< HEAD

/* */

void sh_func(I *instruction)
{
}
=======
void sh_func(I* instruction) {
}  
>>>>>>> 99ac7ac2b874236f40965832fa926c167cca8de9
