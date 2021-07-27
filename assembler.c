#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "input.h"
#include "param.h"

void freeList(node_t *node) {
    while (node->next != NULL) {
        node_t *currNode = node;
        node = node->next;
        free(currNode);
    }
    free(node);
}

int assemble(char *fname) {
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

    unsigned int DC = 0, IC = 100, icf, idf;
    int funcNum, i, j;
    char firstPass = true;
    char tempLine[MAX_LINE_LENGTH] = {0};
    long data_img[1000];
    long instruction_img[1000];
    sym_t *symbol, *symbol_list = calloc(sizeof(sym_t), 1);

    node_t *head, *node;
    flags *flag = (flags *)malloc(sizeof(flags));
    unsigned int instruction_32bit = 0;

    FILE *fp;
    fp = fopen(fname, "r");

    flag->label = false;
    flag->params = false;
    flag->stop = false;
    flag->error = false;
    flag->firstPassDone = false;
    flag->line = 1;

    symbol = symbol_list;

    for (i = 0; i < NUM_OF_REG; i++) { /* registers init */
        regArray[i] = (reg_ptr *)calloc(sizeof(reg_t), 1);
    }

    if (fp) {
        j = 0;
        while (fgets(tempLine, MAX_LINE_LENGTH, fp) != NULL) {
            if (strchr(tempLine, "\n")) {
                printf("Line: %d - Line too long. Max line length is &d", flag->line, MAX_LINE_LENGTH);
            }
            head = getLine(tempLine, flag);
            node = head;
            flag->label = checkIfLabel(node, flag);
            if (flag->label) {
                /* handle label - symbol list */
                node = node->next;
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
                } else if (funcNum < 23) {
                    I *instruction = (I *)calloc(sizeof(I), sizeof(char));
                    if ((instruction = check_i_param(funcNum, node->next, instruction, flag))) {
                        instruction_32bit = i_binary_instruction(instruction);
                        functions[funcNum](instruction);
                    }
                } else if (funcNum < 27) {
                    J *instruction = (J *)calloc(sizeof(J), sizeof(char));
                    if ((instruction = check_j_param(funcNum, node->next, instruction, flag))) {
                        instruction_32bit = j_binary_instruction(instruction);
                        functions[funcNum](instruction);
                    }
                } else if (funcNum == NUM_OF_FUNC + 1) {
                    printf("\nLine: %d - unrecognized instruction <%s>", flag->line, node->val);
                    /* undifined function */
                }
                instruction_img[j] = instruction_32bit; /* insert instruction to memory image */
                freeList(head);
                flag->line += 1;
                IC += 4;
            }
        }
    }

    if (flag->firstPassDone) {
        printf("first Pass Done");
        /* second pass things */
    }

    printf("\n");
    return 1;
}

int main(int argc, char *argv[]) {
    int i;
    /* To break line if needed */
    char succeeded = true;
    /* Process each file by arguments */
    for (i = 1; i < argc; ++i) {
        /* if last process failed and there's another file, break line: */
        if (!succeeded) puts("");
        /* foreach argument (file name), send it for full processing. */
        succeeded = assemble(argv[i]);
        /* Line break if failed */
    }
    return 0;
}