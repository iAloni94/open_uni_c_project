#include "assembler.h"
#include "input.h"
#include "param.h"

void addi_func(I *instruction) {
    unsigned int rs_index = instruction->rs;
    unsigned int immed = instruction->immed;
    unsigned int rs_val = *(regArray[rs_index]);
    unsigned int res = immed + rs_val;
    *(regArray[instruction->rt]) = res;
}

void subi_func(node_t *input) {}
void andi_func(node_t *input) {}
void ori_func(node_t *input) {}
void nori_func(node_t *input) {}
void bne_func(node_t *input) {}
void beq_func(node_t *input) {}
void blt_func(node_t *input) {}
void bgt_func(node_t *input) {}
void lb_func(node_t *input) {}
void sb_func(node_t *input) {}
void lw_func(node_t *input) {}
void sw_func(node_t *input) {}
void lh_func(node_t *input) {}
void sh_func(node_t *input) {}