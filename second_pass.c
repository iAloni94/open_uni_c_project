#include <stdlib.h>
#include <string.h>

#include "global.h"

#define CHECK_BIT(v, n) ((v)&1 << n) ? true : false /* checks if nth bit of v from right is true or false */
#define MASK_1_BTYE 0xFFFF                          /* ..00 0000 1111 1111 1111 1111 */

void secondPass(unsigned int ICF, unsigned int DCF, unsigned int *code_img, sym_t *symbol_list_head, flags *flag, FILE *fp) {
    char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    char *directives[NUM_OF_DIR] = {".db", ".dh", ".dw", ".asciz", ".extern", ".entry"};

    int funcNum, dirNum, i, codeCounter = 0;
    node_t *head, *node;
    sym_t *temp_sym = symbol_list_head;

    if (flag->firstPass) { /* second pass */

        rewind(fp);

        flag->lastLine = false;
        flag->secondPass = true;
        flag->line = 1;

        while (!(flag->lastLine)) {
            head = getLine(fp, flag);
            if (head == NULL)
                continue;
            else {
                node = head;
                if (isLabel(node, flag, symbol_list_head)) { /* skip if label is found */
                    node = node->next;
                }

                dirNum = NUM_OF_DIR;
                for (i = 0; i < NUM_OF_DIR; i++) {
                    if (!strcmp(node->val, directives[i])) {
                        dirNum = i;
                        flag->direction = true;
                    }
                }

                if (dirNum == ent) { /* handle entry directive */
                    ent_handler(symbol_list_head, node, flag);
                    freeInputList(head);
                    flag->line += 1;
                    continue;
                } else if (flag->direction) { /* skip other directions */
                    freeInputList(head);
                    flag->line += 1;
                    flag->direction = false;
                    continue;
                }

                funcNum = NUM_OF_FUNC;
                for (i = 0; i < NUM_OF_FUNC; i++) { /* step 7 */
                    if (!strcmp(node->val, functionName[i])) {
                        funcNum = i;
                        break;
                    }
                }
                if (funcNum >= beq && funcNum <= bgt) {
                    unsigned int label_address;

                    while (node->next != NULL) node = node->next; /* go to last node because thats where the label is */

                    label_address = getSymbolAddress(node->val, symbol_list_head);
                    if (!label_address) {
                        printf("\nLine: %d -  label was not declared!", flag->line);
                        flag->secondPass = false;
                    }

                    while (temp_sym != NULL && temp_sym->name != NULL) { /* check if label is external */
                        if (temp_sym->address == label_address) {
                            if (!strcmp(temp_sym->attribute, "external")) {
                                printf("Line: %d -  Conditional branching does not accept external labels", flag->line);
                                flag->secondPass = false;
                            }
                        }
                        temp_sym = temp_sym->next;
                    }
                    if (flag->secondPass) {
                        unsigned int this_address = (code_img[codeCounter] & MASK_1_BTYE); /* get the address of the instruction itself (rightmost 16 bits) */
                        int new_address = label_address - this_address;                    /* new address to be entered to code image */
                        new_address = new_address & MASK_1_BTYE;                           /* get rightmost 26 bits of new address */
                        code_img[codeCounter] = code_img[codeCounter] & ~MASK_1_BTYE;      /* turn off bit 0-16 of original machine code */
                        code_img[codeCounter] = code_img[codeCounter] | new_address;       /* enter new address */
                    }
                }

                if (funcNum >= jmp && funcNum <= call) {
                    if (!CHECK_BIT(code_img[codeCounter], 25)) { /* if reg bit is off - update address field in code image */
                        unsigned int address = getSymbolAddress(node->next->val, symbol_list_head);
                        code_img[codeCounter] = code_img[codeCounter] | address;

                        /* if external... step 8 */
                    }
                }
            }
            freeInputList(head);
            codeCounter++;
            flag->line += 1;
        } /* end while */
    }
}