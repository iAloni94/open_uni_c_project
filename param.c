/*
* Here we check if the operands for each function are correct.
* Functions are devided into 3 types: R, I and J.
* Each type is devided into sub categories, each demands a different number and type of operands.
* In this file, operands are parsed and if all goes well, an instruction coded in 32 bits is created.
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"


R *check_r_param(int funcNum, node_t *input, R *instruction, flags *flag) {
    char rs = false, rt = false, rd = false, temp;

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
        temp = getReg(input, flag);
        if (temp != NUM_OF_REG) {
            instruction->rs = temp;
            rs = true;
            input = input->next;

            temp = getReg(input, flag);
            if (temp != NUM_OF_REG) {
                instruction->rt = temp;
                rt = true;
                input = input->next;

                temp = getReg(input, flag);
                if (temp != NUM_OF_REG) {
                    instruction->rd = temp;
                    rd = true;
                    input = input->next;
                }
            }
        }

        if (!rs || !rs || !rt) {
            printf("\nLine: %d - Missing operands", flag->line);
            flag->firstPass = false;
            return NULL;
        } else if (input != NULL) {
            flag->firstPass = false;
            printf("\nLine: %d - extraneous operand", flag->line);
            return NULL;
        } else {
            return instruction;
        }
    } else { /* copy functions - 2 parameters rs + rd */
        temp = getReg(input, flag);
        if (temp != NUM_OF_REG) {
            instruction->rs = temp;
            rs = true;
            input = input->next;

            temp = getReg(input, flag);
            if (temp != NUM_OF_REG) {
                instruction->rd = temp;
                rd = true;
                input = input->next;
            }
        }

        if (!rs || !rd) {
            printf("Line: %d - Missing operand", flag->line);
            flag->firstPass = false;
            return NULL;
        } else if (input != NULL) {
            flag->firstPass = false;
            printf("\nLine: %d - extraneous operand", flag->line);
            return NULL;
        } else {
            return instruction;
        }
    }
}

I *check_i_param(int funcNum, node_t *input, I *instruction, flags *flag) {
    char param1 = false, param2 = false, temp;
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
    /* immed is a 16bit signed integer. its range is -32,768 to +32,767 */
    instruction->immed = atoi(input->next->val);

    if (funcNum <= nori) { /* addi to nori  - 3 parameters */
        temp = getReg(input, flag);
        if (temp != NUM_OF_REG) {
            instruction->rs = temp;
            param1 = true;
            input = input->next->next;

            temp = getReg(input, flag);
            if (temp != NUM_OF_REG) {
                instruction->rt = temp;
                param2 = true;
                input = input->next;
            }
        }
    } else if (funcNum <= beq) {
        /* beq bne to bgt */
        /* beq - check if the value in register "rs" is -- Equals -- to the value in register "rt" */
        /* bne - check if the value in register "rs" is -- Not Equals-- to the value in register "rt" */
        /* blt - check if the value in register "rs" is -- Lower Than -- the value in register "rt" */
        /* bgt - check if the value in register "rs" is -- Greater Than -- to the value in register "rt" */
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

J *check_j_param(int funcNum, node_t *input, J *instruction, flags *flag, sym_t *symbol) {
    char temp[MAX_LINE_LENGTH] = {0};
    node_t *tempNode = calloc(sizeof(node_t), 1);

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

    input = input->next;
    if (input->next != NULL) {
        printf("\nLine: %d - Illegal parameter. extraneous operand", flag->line);
        flag->firstPass = false;
        free(tempNode);
        return NULL;
    }

    if (instruction->opcode == jmp_opcode) { /* if jmp function, check for register as parameter */
        if (strchr(input->val, '$')) {
            instruction->reg = true;
            instruction->address = getReg(input, flag);
            if (instruction->address != NUM_OF_REG) {
                free(tempNode);
                return instruction;
            } else {
                free(tempNode);
                return NULL;
            }
        }
    }

    strcpy(temp, input->val);
    strcat(temp, ":");
    tempNode->val = temp;
    if (isLabel(tempNode, flag, symbol)) { /* if operand is label */
        unsigned int tempAddress = getSymbolAddress(input->val, symbol);
        instruction->address = tempAddress;
    }
    free(tempNode);
    return instruction;
}

unsigned int getReg(node_t *node, flags *flag) {
    char *registerList[NUM_OF_REG] = {
        "$0", "$1", "$2", "$3",
        "$4", "$5", "$6", "$7",
        "$8", "$9", "$10", "$11",
        "$12", "$13", "$14", "$15",
        "$16", "$17", "$18", "$19",
        "$20", "$21", "$22", "$23",
        "$24", "$25", "$26", "$27",
        "$28", "$29", "$30", "$31"};
    int i;

    /*
    * if i=0, registerList[i] = $0. if i=1, registerList[i] = $1. if i=28, registerList[i] = $28. if i=n, registerList[i] = $n
    * this way, as soon as our input value == registerList[i], we know which register it is by looking at i
    */
    if (node != NULL) {
        if (strchr(node->val, '$')) {
            for (i = 0; i < NUM_OF_REG; i++) {
                if (!strcmp(node->val, registerList[i])) {
                    return i;
                }
            }
            printf("\nLine: %d - Regsiter \"%s\" out of range", flag->line, node->val);
            flag->firstPass = false;
        } else {
            flag->firstPass = false;
            printf("\nLine: %d - Illegal parameter", flag->line);
        }
    }
    return NUM_OF_REG;
}