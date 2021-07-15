#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "input.h"

#define NUM_OF_FUNC 27

int main(int argc, char *argv[]) {
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

    int funcNum, i = 0;
    node_t *head;
    node_t *node;
    flags *flag = (flags *)malloc(sizeof(flags));

    flag->label = false;
    flag->params = false;
    flag->stop = false;
    
    if (argc <= 1) {
        printf("No files were detected");
    } else {
        FILE *fp;
        fp = fopen(argv[1], "r");
        while (!(flag->stop))
            if (fp) {
                head = getLine(fp);
                node = head;
                flag->label = checkIfLabel(node);
                node = node->next;
                if (head) {
                    funcNum = 27;
                    for (i = 0; i < 27; i++) {
                        if (strcmp(node->val, functionName[i]) == 0) {
                            funcNum = i;
                            flag->params = checkParam(funcNum, node->next);
                            break;
                        }
                    }

                    while (head->next != NULL) { /* free current line memory */
                        node_t *currNode = head;
                        head = head->next;
                        free(currNode);
                    }
                    free(head);
                } 
            }
    }
    return 1;
}