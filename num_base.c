#include "input.h"
#include "param.h"

unsigned int r_binary_instruction(R *instruction) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode;
    my_32bit |= (temp << 26); /*move 26 bits left, now our 6 bits of opcode are the most significant*/

    temp = instruction->rs;
    my_32bit |= (temp << 21); /* why 21 ? we had 6 bits from the left, now we need to fit 5 bits, so we need to start from 5 bits less from where we are at now so thats 26-5 = 21*/

    temp = instruction->rt;
    my_32bit |= (temp << 16); /* why 16 ? same thing... we need 5 bits from the last 5 bits, therefore 21-5 = 16 */

    temp = instruction->rd;
    my_32bit |= (temp << 11); /* why 11 ? same thing we used 5 bits now we need more 5 bits starting from last 5 bits therefore 16-5 = 11 */

    temp = instruction->funct;
    my_32bit |= (temp << 6); /* why 6 ? same thing we used 5 bites now we need more 5 its therefore 11-5 = 6 */

    temp = instruction->notused;
    my_32bit |= (temp); /* these are the remaining 6 bits, we dont need to move them anywhere, we just add them */
    return my_32bit;
}

unsigned int i_binary_instruction(I *instruction, char type) {
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
unsigned int j_binary_instruction(J *instruction, char type) {
    unsigned int my_32bit = 0;
    unsigned int temp;

    temp = instruction->opcode;
    my_32bit |= (temp << 26);

    temp = instruction->reg;
    my_32bit |= (temp << 25);

    temp = instruction->adress;
    my_32bit |= (temp);

    return my_32bit;
}