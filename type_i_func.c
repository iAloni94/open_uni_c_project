#include "assembler.h"
#include "param.h"


void addi_func(I *instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = immed + rs_val;
    *(regArray[instruction->rt]) = res;
}

void subi_func(I* instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = immed - rs_val;
    *(regArray[instruction->rt]) = res;
}
void andi_func(I* instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = immed & rs_val;
    *(regArray[instruction->rt]) = res;
}
void ori_func(I* instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = immed | rs_val;
    *(regArray[instruction->rt]) = res;
}
void nori_func(I* instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = ~(immed | rs_val);
    *(regArray[instruction->rt]) = res;
}
void bne_func(I* instruction) {
}
void beq_func(I* instruction) {
}
void blt_func(I* instruction) {
}
void bgt_func(I* instruction) {
}
void lb_func(I* instruction) {
}
void sb_func(I* instruction) {
}
void lw_func(I* instruction) {
}
void sw_func(I* instruction) {
}
void lh_func(I* instruction) {
}
void sh_func(I* instruction) {
}