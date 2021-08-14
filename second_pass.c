#include <stdlib.h>
#include <string.h>

#include "global.h"

#define CHECK_BIT(v, n) ((v)&1 << n) ? true : false /* checks if nth bit of v from right is true or false */
#define MASK_1_BTYE 0xFFFF                          /* ..00 0000 1111 1111 1111 1111 */

void secondPass(unsigned int ICF, unsigned int DCF, unsigned int *code_img, sym_t *symbol_list_head, flags *flag, FILE *fp, ext_t *ext_list_head) {
    char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    char *directives[NUM_OF_DIR] = {".db", ".dh", ".dw", ".asciz", ".extern", ".entry"};

    int funcNum, dirNum, i, codeCounter = 0, IC = INITIAL_MEM_ADDRESS; /* IC is set again for updating the external table */
    node_t *head, *node;
    sym_t *temp_sym = symbol_list_head;
    ext_t *ext_node = ext_list_head; /* External table */

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
                    flag->direction = false;
                    flag->line += 1;
                    continue;
                } else if (flag->direction) { /* skip other directions */
                    freeInputList(head);
                    flag->line += 1;
                    flag->direction = false;
                    continue;
                }

                funcNum = NUM_OF_FUNC; 
                for (i = 0; i < NUM_OF_FUNC; i++) {
                    if (!strcmp(node->val, functionName[i])) {
                        funcNum = i;
                        break;
                    }
                }


                if (funcNum >= beq && funcNum <= bgt) {
                    unsigned int label_address;

                    while (node->next != NULL) node = node->next; /* go to last node because thats where the label is */

                    temp_sym = getSymbol(node->val, symbol_list_head);
                    if (temp_sym != NULL) { 
                        label_address = temp_sym->address;
                        if (!temp_sym) {
                            printf("\nLine: %d -  label was not declared!", flag->line);
                            flag->secondPass = false;
                        }
                    } else {
                        printf("\nLine: %d - No reference to \"%s\" label", flag->line, node->val);
                        flag->secondPass = false;
                    }
                    temp_sym = symbol_list_head;

                    while (temp_sym != NULL && temp_sym->name != NULL && flag->secondPass) { /* check if label is external */
                        if (temp_sym->address == label_address) {
                            if (!strcmp(temp_sym->attribute, "external")) {
                                printf("\nLine: %d -  Conditional branching does not accept external labels", flag->line);
                                flag->secondPass = false;
                            }
                        }
                        temp_sym = temp_sym->next;
                    }
                    if (flag->secondPass) {
                        calcDistance(&(code_img[codeCounter]), label_address);
                    }
                }

                if (funcNum >= jmp && funcNum <= call) {
                    if (!CHECK_BIT(code_img[codeCounter], REG_BIT)) { /* if reg bit is off - update address field in code image */
                        temp_sym = getSymbol(node->next->val, symbol_list_head);
                        if (temp_sym != NULL) {
                            code_img[codeCounter] = code_img[codeCounter] | temp_sym->address;
                        } else {
                            printf("\nLine: %d - No reference to \"%s\" label", flag->line, node->next->val);
                            flag->secondPass = false;
                        }

                        if ((temp_sym = getSymbol(node->next->val, symbol_list_head))) {
                            if (!strcmp(temp_sym->attribute, "external")) {
                                ext_node->name = temp_sym->name;
                                ext_node->address = IC;
                                ext_node->next = calloc(sizeof(ext_t), 1);
                                ext_node = ext_node->next;
                            }
                        }
                    }
                }
            }
            freeInputList(head);
            codeCounter++;
            flag->line += 1;
            IC += INSTRUCTION_SIZE;
        } /* end while */
    }
}