#include "param.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"

char* registerList[NUM_OF_REG] = {
    "$0", "$1", "$2", "$3",
    "$4", "$5", "$6", "$7",
    "$8", "$9", "$10", "$11",
    "$12", "$13", "$14", "$15",
    "$16", "$17", "$18", "$19",
    "$20", "$21", "$22", "$23",
    "$24", "$25", "$26", "$27",
    "$28", "$29", "$30", "$31"};

R* check_r_param(int funcNum, node_t* input, R* instruction, flags* flag) {
    char rs = false, rt = false, rd = false;
    int i;

    switch (funcNum) { /* opcode */
        case 5:
        case 6:
        case 7:
            instruction->opcode = 1;
            break;
        default:
            break;
    }

    switch (funcNum) { /* funct */
        case add:
        case move:
            instruction->funct = 1;
            break;
        case sub:
        case mvhi:
            instruction->funct = 2;
            break;
        case and:
        case mvlo:
            instruction->funct = 3;
            break;
        case or:
            instruction->funct = 4;
            break;
        case nor:
            instruction->funct = 5;
            break;
        default:
            break;
    }

    if (funcNum <= nor) { /* arithmatics functions - 3 parameters */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (rs == false && !strcmp(input->val, registerList[i])) {
                instruction->rs = i;
                *(input->val) = i;
                rs = true;
                input = input->next;
                i = 0;
                continue;
            }
            if (rt == false && !strcmp(input->val, registerList[i])) {
                instruction->rt = i;
                *(input->val) = i;
                rt = true;
                input = input->next;
                i = 0;
                continue;
            }

            if (rd == false && !strcmp(input->val, registerList[i])) {
                instruction->rd = i;
                *(input->val) = i;
                rd = true;
                input = input->next;
                i = 0;
                continue;
            }
        }
    } else { /* copy functions - 2 parameters rs + rd */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (rs == false && !strcmp(input->val, registerList[i])) {
                instruction->rs = i;
                rs = true;
                input = input->next;
                i = 0;
                continue;
            }
            if (rd == false && !strcmp(input->val, registerList[i])) {
                instruction->rd = i;
                rd = true;
                input = input->next;
                i = 0;
                continue;
            }
        }
    }
    if (input != NULL) {
        flag->firstPass = false;
        printf("\nLine: %d - Illigal parameter. extraneous operand", flag->line);
        return NULL;
    }

    if (funcNum <= 4 && (rs && rt && rd)) {
        return instruction;
    } else if (rs && rd) {
        return instruction;
    } else {
        flag->firstPass = false;
        printf("\nLine: %d - Illigal parameter", flag->line);
        return NULL;
    }
}

I* check_i_param(int funcNum, node_t* input, I* instruction, flags* flag) {
    char param1 = false, param2 = false;
    int i = 0;

    instruction->opcode = funcNum + 2;

    while (i < strlen(input->next->val)) {
        if (!IS_NUM(*(input->next->val + i))) {
            flag->firstPass = false;
            printf("\nLine: %d - Illigal parameter. Immed value should only be an integer", flag->line);
            return NULL;
        }
        i++;
    }
    instruction->immed = atoi(input->next->val);

    if (funcNum <= nori) { /* addi to nori  - 3 parameters */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (param1 == false && !strcmp(input->val, registerList[i])) {
                instruction->rs = i;
                param1 = true;
                input = input->next->next;
                i = 0;
                continue;
            }
            if (param2 == false && !strcmp(input->val, registerList[i])) {
                instruction->rt = i;
                param2 = true;
                input = input->next;
                continue;
            }
        }
    } else if (funcNum <= bgt) { /* bne to bgt */

    } else { /* lb to sh */
    }

    if (param1 && param2) {
        return instruction;
    } else {
        flag->firstPass = false;
        printf("\nLine: %d - Register not in range", flag->line);
        return NULL;
    }
}

J* check_j_param(int funcNum, node_t* input, J* instruction, flags* flag, sym_t* symbol) {
    int i;
    char declared = false;
    node_t* tempNode = calloc(sizeof(node_t), 1);
    char temp[MAX_LINE_LENGTH] = {0};
    strcpy(temp, input->val);
    strcat(temp, ":");
    tempNode->val = temp;

    if (tempNode) {
        if (strcmp(input->val, "stop")) {
            if (input->next->next != NULL) {
                flag->firstPass = false;
                printf("\nLine: %d - Illigal parameter. extraneous operand", flag->line);
                return NULL;
            }
        }

        switch (funcNum) { /* opcode */
            case jmp:      /* jmp */
                instruction->opcode = 30;
                break;
            case la:
                instruction->opcode = 31;
                instruction->reg = 0;
                break;
            case call:
                instruction->opcode = 32;
                instruction->reg = 0;
                break;
            case stop:
                if (input->next != NULL) {
                    flag->firstPass = false;
                    printf("\nLine: %d - Illigal parameter. extraneous operand", flag->line);
                    return NULL;
                }
                instruction->opcode = 63;
                instruction->reg = 0;
                instruction->address = 0;
                break;
            default:
                break;
        }

        if (instruction->opcode != 63) {
            input = input->next;
            if (instruction->opcode == 30) { /* JMP */
                if (strchr(input->val, '$')) {
                    for (i = 0; i < NUM_OF_REG; i++) {
                        if (!strcmp(input->val, registerList[i])) {
                            instruction->reg = true;
                            instruction->address = i;
                            break;
                        }
                    }
                    if (i == NUM_OF_REG) {
                        flag->firstPass = false;
                        printf("\nLine: %d - Register not in range", flag->line);
                        return NULL;
                    }
                }
            }
            if (isLabel(tempNode, flag)) { /* Label */

                instruction->reg = false;

                while (symbol->name != NULL) { /* checking if the label was already declared */
                    if (!strcmp(input->val, symbol->name)) {
                        if (!strcmp(symbol->attribute, "external")) {
                            instruction->address = 0;
                            break;
                        }
                        instruction->address = symbol->address;
                        declared = true;
                        break;
                    }
                    symbol = symbol->next;
                }

                if (!declared) {
                    instruction->address = undef_address;
                }
            }

            free(tempNode);
        }
        return instruction;
    } else {
        printf("Memory allocation error");
        flag->firstPass = false;
        return NULL;
    }
}
