#include "assembler.h"
#include "param.h"
#include "utils.h"
#include "func.h"

void add_func(R *instruction) {
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs + rt;
    regArray[instruction->rd]->val = rd;
}

void sub_func(R *instruction) {
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs - rt;
    regArray[instruction->rd]->val = rd;
}  
void and_func(R *instruction) {
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = rs & rt;
    regArray[instruction->rd]->val = rd;
}
void or_func(R *instruction) {
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = (rs | rt);
    regArray[instruction->rd]->val = rd;
}
void nor_func(R *instruction) {
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rt = (regArray[instruction->rt]->val);
    unsigned int rd = ~(rs | rt);
    regArray[instruction->rd]->val = rd;
}
void move_func(R *instruction) {
    regArray[instruction->rd]->val = (regArray[instruction->rs]->val);
}
void mvhi_func(R *instruction) {
    /* copy bits 16-31 from rs to rd */
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rd = (regArray[instruction->rd]->val);
    for (i = 31; i > 15; i--) {
        char bit = CHECK_BIT(rs, i);
        CHANGE_BIT(rd, bit, i);
    }
    regArray[instruction->rd]->val = rd;
}
void mvlo_func(R *instruction) {
    /* copy bits 0-15 from rs to rd */
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rd = (regArray[instruction->rd]->val);
    for (i = 0; i < 16; i++) {
        char bit = CHECK_BIT(rs, i);
        CHANGE_BIT(rd, bit, i);
    }
    regArray[instruction->rd]->val = rd;
}