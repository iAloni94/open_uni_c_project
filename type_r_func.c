#include "assembler.h"
#include "param.h"
#include "utils.h"

#define CHECK_BIT(reg, n) ((reg)&1 << n) ? true : false        /* checks if bit in nth place from right is true or false */
#define TOGGLE_BIT(reg, n, v) ((reg) ^= v << n) /* toggles nth bit from right to v (0 or 1) */

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
    regArray[instruction->rd] = (regArray[instruction->rs]->val);
}
void mvhi_func(R *instruction) {
    /* copy bits 16-31 from rs to rd */
}
void mvlo_func(R *instruction) {
    /* copy bits 0-15 from rs to rd */
    int i;
    unsigned int rs = (regArray[instruction->rs]->val);
    unsigned int rd = (regArray[instruction->rd]->val);
    for (i = 0; i < 16; i++) {
        char bit = CHECK_BIT(rs, i);
        TOGGLE_BIT(rd, i, 0);
        TOGGLE_BIT(rd, i, bit);
    }
    printf("%d, %d", rs, rd);
    regArray[instruction->rd]->val = rd;
}

void undef_func() {}