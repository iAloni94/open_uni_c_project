/*
* In this file, we will convert the instruction structs into a 32 bit unsigned int (machine code)
*/

#include "instruction.h"
#include "utils.h"

unsigned int r_binary_instruction(R *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode; /* shift result to opcode part of instruction (bit 26-31) */
    my_32bit |= (temp << 26);

    temp = instruction->rs; /* bit 21-25 */
    my_32bit |= (temp << 21);

    temp = instruction->rt; /* bit 16-20 */
    my_32bit |= (temp << 16);

    temp = instruction->rd; /* bit 26-31 */
    my_32bit |= (temp << 11);

    temp = instruction->funct; /* bit 10-16*/
    my_32bit |= (temp << 6);

    temp = instruction->notused; /* bit 0-5*/
    my_32bit |= (temp);
    return my_32bit;
}

unsigned int i_binary_instruction(I *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;
    int immed;

    temp = instruction->opcode; /* shift result to opcode part of instruction (bit 26-31) */
    my_32bit |= (temp << 26);

    temp = instruction->rs; /* (21-25) */
    my_32bit |= (temp << 21);

    temp = instruction->rt; /* (16-20) */
    my_32bit |= (temp << 16);

    immed = instruction->immed; /* (0-15) */
    immed = immed & MASK_1_BTYE;
    my_32bit |= (immed);

    return my_32bit;
}
unsigned int j_binary_instruction(J *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode; /* shift result to opcode part of instruction (bit 26-31) */
    my_32bit |= (temp << 26);

    temp = instruction->reg; /* (25) */
    my_32bit |= (temp << 25);

    temp = instruction->address; /* (0-24) */
    my_32bit |= (temp);

    return my_32bit;
}