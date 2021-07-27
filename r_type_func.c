#include "assembler.h"
#include "param.h"
#include "utils.h"

void add_func(R *instruction) {
    int rs_index = instruction->rs;
    int rt_index = instruction->rt;
    int rd_index = instruction->rt;
    unsigned int rs = *(regArray[rs_index]);
    unsigned int rt = *(regArray[rt_index]);
    unsigned int rd = rs + rt;
    *regArray[rd_index] = rd;
}

void sub_func(R *instruction) {
    int rs_index = instruction->rs;
    int rt_index = instruction->rt;
    int rd_index = instruction->rt;
    unsigned int rs = *(regArray[rs_index]);
    unsigned int rt = *(regArray[rt_index]);
    unsigned int rd = rs - rt;
    *regArray[rd_index] = rd;
}
void and_func(R *instruction) {
    int rs_index = instruction->rs;
    int rt_index = instruction->rt;
    int rd_index = instruction->rt;
    unsigned int rs = *(regArray[rs_index]);
    unsigned int rt = *(regArray[rt_index]);
    unsigned int rd = rs & rt;
    *regArray[rd_index] = rd;
}
void or_func(R *instruction) {
    int rs_index = instruction->rs;
    int rt_index = instruction->rt;
    int rd_index = instruction->rt;
}
void nor_func(R *instruction) {
    int rs_index = instruction->rs;
    int rt_index = instruction->rt;
    int rd_index = instruction->rt;
    unsigned int rs = *(regArray[rs_index]);
    unsigned int rt = *(regArray[rt_index]);
    unsigned int rd = ~(rs | rt);
    *regArray[rd_index] = rd;
}
void move_func(R *instruction) {
}
void mvhi_func(R *instruction) {
}
void mvlo_func(R *instruction) {
}

void undef_func() {}