#include <stdlib.h>
#include <string.h>

#include "global.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void firstPass(unsigned int *IC, unsigned int *ICF, unsigned int *DC, unsigned int *DCF, unsigned int *code_img, dir_t *data_img, sym_t *symbol_list_head, flags *flag, FILE *fp) {
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
    dir_t *this_data;
    node_t *head, *node;
    unsigned int first_pass_32bit = 0;

    if (flag == NULL || symbol_list_head == NULL || data_img == NULL) {
        printf("Memory allocation error");
        exit(0);
    } else {
        this_data = data_img;
        while (!(flag->lastLine)) {   /* first pass */
            head = getLine(fp, flag); /* each node in input list contains a single word */
            if (head == NULL) {
                flag->line += 1;
            } else {
                node = head;

                if (flag->label) {                                                      /* if ':' was found, there is a suspected label */
                    if ((flag->label = isLabel(node, flag, symbol_list_head))) {        /* raise flag if label found */
                        flag->label = !(isDeclared(node->val, symbol_list_head, flag)); /* check if label was already declared*/
                    }
                    node = node->next;
                }

                dirNum = NUM_OF_DIR;
                for (i = 0; i < NUM_OF_DIR; i++) {
                    if (!strcmp(node->val, directives[i])) {
                        dirNum = i;
                        flag->direction = true;
                        break;
                    }
                }
                if (!flag->direction) { /* if instruction */
                    funcNum = NUM_OF_FUNC;
                    for (i = 0; i < NUM_OF_FUNC; i++) {
                        if (!strcmp(node->val, functionName[i])) {
                            funcNum = i;
                            break;
                        }
                    }

                    if (funcNum <= mvlo) { /* R type function */
                        R *instruction = (R *)calloc(sizeof(R), 1);
                        if (instruction == NULL) {
                            flag->firstPass = false;
                            printf("\nMemory allocation error");
                            exit(0);
                        }
                        if ((instruction = get_r_instruction(funcNum, node->next, instruction, flag))) {
                            first_pass_32bit = r_binary_instruction(instruction);
                            free(instruction);
                        }
                    } else if (funcNum <= sh) { /* I type function */
                        I *instruction = (I *)calloc(sizeof(I), 1);
                        if (instruction == NULL) {
                            flag->firstPass = false;
                            printf("\nMemory allocation error");
                            exit(0);
                        }
                        if ((instruction = get_i_instruction(funcNum, node->next, instruction, flag, symbol_list_head, *IC))) {
                            first_pass_32bit = i_binary_instruction(instruction);
                            free(instruction);
                        }
                    } else if (funcNum <= stop) { /* J type function */
                        J *instruction = (J *)calloc(sizeof(J), 1);
                        if (instruction == NULL) {
                            flag->firstPass = false;
                            printf("\nMemory allocation error");
                            exit(0);
                        }
                        if ((instruction = get_j_instruction(funcNum, node, instruction, flag, symbol_list_head))) {
                            first_pass_32bit = j_binary_instruction(instruction);
                            free(instruction);
                        }
                    } else if (funcNum == NUM_OF_FUNC) { /* undefined function */
                        flag->firstPass = false;
                        printf("\nLine: %d - Unrecognized instruction \"%s\"", flag->line, node->val);
                        freeInputList(head);
                        flag->line += 1;
                        (*IC) += INSTRUCTION_SIZE;
                        continue;
                    }

                    if (flag->label) { /* if found, inserts label into symbol table. each node is a label */
                        insertLabel(symbol_list_head, head, flag, *IC, *DC);
                    }
                    freeInputList(head);
                    code_img[codeCounter] = first_pass_32bit; /* insert binary instruction to memory image */
                    codeCounter++;
                    (*IC) += INSTRUCTION_SIZE;
                    flag->line++;
                } else {               /* if directive */
                    if (flag->label) { /* if label is found, inserts label into symbol table. each node is a label */
                        if (dirNum == ext || dirNum == ent) {
                            printf(ANSI_COLOR_RED "\nWarning! " ANSI_COLOR_RESET);
                            printf("Line: %d - Label before external or entry directive ignored", flag->line);
                        } else {
                            insertLabel(symbol_list_head, head, flag, *IC, *DC);
                        }
                        flag->label = false;
                    }
                    switch (dirNum) {
                        case db:
                            this_data = save_byte(node->next, this_data, DC, flag);
                            break;
                        case dh:
                            this_data = save_half_word(node->next, this_data, DC, flag);
                            break;
                        case dw:
                            this_data = save_word(node->next, this_data, DC, flag);
                            break;
                        case asciz:
                            this_data = save_str(node->next, this_data, DC, flag);
                            break;
                        case ext:
                            ext_handler(symbol_list_head, node->next, flag, *IC, *DC);
                            break;
                        default:
                            break;
                    }

                    freeInputList(head);
                    flag->direction = false;
                    flag->line += 1;
                }
            }
        } /* while loop */
        (*ICF) = (*IC);
        (*DCF) = (*DC);
        if (symbol_list_head->name != NULL) {
            updateSymbolAddress(symbol_list_head, *ICF);
        }
    }
}