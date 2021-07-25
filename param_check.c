#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "input.h"
#include "param.h"

char* registerList[] = {
    "$0", "$1", "$2", "$3",
    "$4", "$5", "$6", "$7",
    "$8", "$9", "$10", "$11",
    "$12", "$13", "$14", "$15",
    "$16", "$17", "$18", "$19",
    "$20", "$21", "$22", "$23",
    "$24", "$25", "$26", "$27",
    "$28", "$29", "$30", "$31",
    "$32"};

R* check_r_param(int funcNum, node_t* input, R* instruction, flags* flag) {
    char param1 = false, param2 = false, param3 = false;
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

    if (funcNum <= 4) { /* arithmatics functions - 3 parameters */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (param1 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rs = i;
                *(input->val) = i;
                param1 = true;
                input = input->next;
                i = 0;
                continue;
            }
            if (param2 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rt = i;
                *(input->val) = i;
                param2 = true;
                input = input->next;
                i = 0;
                continue;
            }

            if (param3 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rd = i;
                *(input->val) = i;
                param3 = true;
                input = input->next;
                i = 0;
                continue;
            }
        }
    } else { /* copy functions - 2 parameters */
        for (i = 0; i < NUM_OF_REG && input; i++) {
            if (param1 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rs = i;
                param1 = true;
                input = input->next;
                i = 0;
                continue;
            }
            if (param2 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rt = i;
                param2 = true;
                input = input->next;
                i = 0;
                continue;
            }
        }
    }

    switch (funcNum) { /* funct */
        case 0:
        case 5:
            instruction->funct = 1;
            break;
        case 1:
        case 6:
            instruction->funct = 2;
            break;
        case 2:
        case 7:
            instruction->funct = 3;
            break;
        case 3:
            instruction->funct = 4;
            break;
        case 4:
            instruction->funct = 5;
            break;
        default:
            break;
    }

    if (funcNum <= 4 && (param1 && param2 && param3)) {
        return instruction;
    } else if (param1 && param2) {
        return instruction;
    } else {
        flag->error = true;
        printf("\nLine: %d - Register not in range", flag->line);
        return NULL;
    }
}

I* check_i_param(int funcNum, node_t* input, I* instruction, flags* flag) {
    char param1 = false, param2 = false;
    int i;

    instruction->opcode = funcNum + 2;
    if (strchr(input->next->val, '.')) {
        printf("Line : %d", flag->line);
        printf("This assembler support only integers");
        flag->error = true;
        return NULL;
    }
    instruction->immed = atoi(input->next->val);

    if (funcNum <= 12) {                    /* addi to nori */
        for (i = 0; i < 32 && input; i++) { /* 32 is number of registerd */
            if (param1 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rs = i;
                param1 = true;
                input = input->next->next;
                i = 0;
                continue;
            }
            if (param2 == false && strcmp(input->val, registerList[i]) == 0) {
                instruction->rt = i;
                param2 = true;
                input = input->next;
                continue;
            }
        }
    } else if (funcNum <= 16) { /* bne to bgt */

    } else { /* lb to sh */
    }

    if (param1 && param2) {
        return instruction;
    } else {
        flag->error = true;
        printf("\nLine: %d - Register not in range", flag->line);
        return NULL;
    }
}

J* check_j_param(int funcNum, node_t* input, J* instruction, flags* flag) {
    return instruction;
}