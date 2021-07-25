#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "input.h"
#include "param.h"

void freeList(node_t *node) {
    while (node->next != NULL) { /* free current line memory */
        node_t *currNode = node;
        node = node->next;
        free(currNode);
    }
    free(node);
}

int main(int argc, char *argv[]) {
    char *directions[NUM_OF_DIR] = {".dd", ".dw", ".db", "asciz", ".extern", ".entry"};

    char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    char (*functions[NUM_OF_FUNC + 1])() = {
        add_func, sub_func, and_func, or_func,
        nor_func, move_func, mvhi_func, mvlo_func,
        addi_func, subi_func, andi_func, ori_func,
        nori_func, bne_func, beq_func, blt_func,
        bgt_func, lb_func, sb_func, lw_func,
        sw_func, lh_func, sh_func, jmp_func,
        la_func, call_func, stop_func, undef_func};

    unsigned int DC = 0, IC = 100;
    int funcNum, i, line = 1;
    node_t *head;
    node_t *node;
    flags *flag = (flags *)malloc(sizeof(flags));
    unsigned int instruction_32bit = 0;

    for (i = 0; i < NUM_OF_REG; i++) { /* reg init */
        regArray[i] = (reg_ptr *)calloc(sizeof(reg_t), 1);
    }

    flag->label = false;
    flag->params = false;
    flag->stop = false;
    flag->error = false;
    flag->line = 1;

    if (argc <= 1) {
        printf("No files were detected");
    } else {
        FILE *fp;
        fp = fopen(argv[1], "r");
        while (!(flag->stop)) {
            if (fp) {
                head = getLine(fp, flag);
                node = head;
                flag->label = checkIfLabel(node, flag);
                if (flag->label) {
                    /* handle label */
                    node = node->next;
                }
                if (!strcmp(head->val, "stop")) {
                    flag->stop = 1;
                    free(head);
                    break;
                }
                if (head) {
                    for (i = 0; i < NUM_OF_DIR + 1; i++) {
                        if (strcmp(node->val, directions[i]) == 0) {
                            /* handle directions - directions.c */
                        }
                    }

                    funcNum = NUM_OF_FUNC + 1;
                    for (i = 0; i < NUM_OF_FUNC + 1; i++) {
                        if (strcmp(node->val, functionName[i]) == 0) {
                            funcNum = i;
                            break;
                        }
                    }

                    if (funcNum < 8) {
                        R *instruction = (R *)calloc(sizeof(R), sizeof(char));
                        if ((instruction = check_r_param(funcNum, node->next, instruction, flag))) {
                            instruction_32bit = r_binary_instruction(instruction);
                            functions[funcNum](instruction);
                        }
                    } else if (funcNum <= 23) {
                        I *instruction = (I *)calloc(sizeof(I), sizeof(char));
                        if ((instruction = check_i_param(funcNum, node->next, instruction, flag))) {
                            instruction_32bit = i_binary_instruction(instruction);
                            functions[funcNum](instruction);
                        }
                    } else if (funcNum <= 27) {
                        J *instruction = (J *)calloc(sizeof(J), sizeof(char));
                        if ((instruction = check_j_param(funcNum, node->next, instruction, flag))) {
                            instruction_32bit = j_binary_instruction(instruction);
                            functions[funcNum](instruction);
                        }
                    } else if (funcNum == NUM_OF_FUNC + 1) {
                        /* undifined function */
                    }
                    freeList(head);
                    flag->line += 1;
                }
            }
        }
    }
    return 1;
}