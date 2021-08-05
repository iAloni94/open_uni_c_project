#include "instruction.h"
 
unsigned int r_binary_instruction(R *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode;
    my_32bit |= (temp << 26);

    temp = instruction->rs;
    my_32bit |= (temp << 21); 

    temp = instruction->rt; 
    my_32bit |= (temp << 16); 

    temp = instruction->rd;
    my_32bit |= (temp << 11); 

    temp = instruction->funct;
    my_32bit |= (temp << 6);

    temp = instruction->notused;
    my_32bit |= (temp); 
    return my_32bit;
}

unsigned int i_binary_instruction(I *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode;
    my_32bit |= (temp << 26);
    
    temp = instruction->rs;
    my_32bit |= (temp << 21);

    temp = instruction->rt;
    my_32bit |= (temp << 16);

    temp = instruction->immed;
    my_32bit |= (temp);

    return my_32bit;
}
unsigned int j_binary_instruction(J *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode;
    my_32bit |= (temp << 26);

    temp = instruction->reg;
    my_32bit |= (temp << 25);

    temp = instruction->address;
    my_32bit |= (temp); 

    return my_32bit;
}