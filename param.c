#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"

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
            instruction->opcode = mv_opcode;
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
            if (strchr(input->val, '$')) {
                if (rs == false && !strcmp(input->val, registerList[i])) {
                    instruction->rs = i;
                    rs = true;
                    input = input->next;
                    i = 0;
                    continue;
                }
                if (rt == false && !strcmp(input->val, registerList[i])) {
                    instruction->rt = i;
                    rt = true;
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
            } else {
                flag->firstPass = false;
                printf("\nLine: %d - Illegal parameter name", flag->line);
                return NULL;
            }
        }
        if (i == NUM_OF_REG) {
            flag->firstPass = false;
            printf("\nLine: %d - Register out of range (0-31)", flag->line);
            return NULL;
        }
    } else { /* copy functions - 2 parameters rs + rd */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (strchr(input->val, '$')) {
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
            } else {
                flag->firstPass = false;
                printf("\nLine: %d - Illegal parameter name", flag->line);
                return NULL;
            }
        }
        if (i == NUM_OF_REG) {
            flag->firstPass = false;
            printf("\nLine: %d - Register out of range", flag->line);
            return NULL;
        }
    }
    if (input != NULL) {
        flag->firstPass = false;
        printf("\nLine: %d - extraneous operand", flag->line);
        return NULL;
    }

    if (funcNum <= 4 && (rs && rt && rd)) {
        return instruction;
    } else if (rs && rd) {
        return instruction;
    } else {
        flag->firstPass = false;
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
            printf("\nLine: %d - Illegal parameter. Immed value should only be an integer", flag->line);
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
    char temp[MAX_LINE_LENGTH] = {0};
    node_t* tempNode = calloc(sizeof(node_t), 1);

    switch (funcNum) { /* opcode */
        case jmp:
            instruction->opcode = jmp_opcode;
            break;
        case la:
            instruction->opcode = la_opcode;
            instruction->reg = 0;
            break;
        case call:
            instruction->opcode = call_opcode;
            instruction->reg = 0;
            break;
        case stop:
            if (input->next != NULL) {
                flag->firstPass = false;
                printf("\nLine: %d - Illegal parameter. extraneous operand", flag->line);
                free(tempNode);
                return NULL;
            }
            instruction->opcode = stop_opcode;
            instruction->reg = 0;
            instruction->address = 0;
            free(tempNode);
            return instruction;
        default:
            break;
    }

    if (instruction->opcode != stop_opcode) {
        input = input->next;
        if (instruction->opcode == jmp_opcode) { /* if jmp function, check for register as parameter */
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
                    printf("\nLine: %d - Register out of range (0-31)", flag->line);
                    return NULL;
                }
            }
            return instruction;
        }
        strcpy(temp, input->val);
        strcat(temp, ":");
        tempNode->val = temp;
        if (isLabel(tempNode, flag, symbol)) { /* if operand is label */
            unsigned int tempAddress = getSymbolAddress(input->val, symbol);
            if (tempAddress) {
                instruction->address = tempAddress;
            } else {
                instruction->address = 0;
            }
            free(tempNode);
            return instruction;
        } else {
            flag->firstPass = false;
            printf("\nLine: %d - Illegal parameter name", flag->line);
            free(tempNode);
            return NULL;
        }
    }
    return instruction;
}
