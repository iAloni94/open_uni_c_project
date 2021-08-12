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

#define IN_RANGE_IMMED(n) n >= SHRT_MIN &&n <= SHRT_MAX ? true : false /* chec if n is in 16 bit range */

/*
* This method parses the instruction for R type function.
* there are 2 types of R function, arithmatics and copy.
* each type requires different number of opernads.
* It check if all the operands and parameters are valid.
* if all operands check out, a 32 bit instruction is built based on them
*/
R *check_r_param(int funcNum, node_t *input, R *instruction, flags *flag) {
    char rs = false, rt = false, rd = false, temp;

    switch (funcNum) { /* opcode */
        case move:
        case mvhi:
        case mvlo:
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

    if (funcNum <= nor) { /* arithmatics functions - 3 operands rs + rd + rt */
        temp = getReg(input, flag);
        if (temp <= NUM_OF_REG) {
            instruction->rs = temp;
            rs = true;
            input = input->next;

            temp = getReg(input, flag);
            if (temp <= NUM_OF_REG) {
                instruction->rt = temp;
                rt = true;
                input = input->next;

                temp = getReg(input, flag);
                if (temp <= NUM_OF_REG) {
                    instruction->rd = temp;
                    rd = true;
                    input = input->next;
                }
            }
        }

        if ((!rs || !rd || !rt) && temp <= NUM_OF_REG) { /* if any of the 3 required operands is missing */
            printf("\nLine: %d - Missing operands", flag->line);
            flag->firstPass = false;
            return NULL;
        } else if (input != NULL) { /* if all operands were found but there is more input after */
            if (strlen(input->val) != 0) {
                printf("\nLine: %d - Extraneous operand", flag->line);
            } else {
                printf("\nLine: %d - Extraneous comma", flag->line);
            }
            flag->firstPass = false;
            return NULL;
        }
        return instruction;
    } else { /* copy functions - 2 parameters rs + rd */
        temp = getReg(input, flag);
        input = input->next;

        if (temp <= NUM_OF_REG) {
            instruction->rs = temp;
            rs = true;

            temp = getReg(input, flag);
            if (temp <= NUM_OF_REG) {
                instruction->rd = temp;
                rd = true;
                input = input->next;
            }
        }

        if ((!rs || !rd) && temp <= NUM_OF_REG) { /* if any of the 2 required operands is missing */
            printf("\nLine: %d - Missing operand", flag->line);
            flag->firstPass = false;
            return NULL;
        } else if (input != NULL) { /* if all operands were found but there is more input after */
            if (strlen(input->val) != 0) {
                printf("\nLine: %d - Extraneous operand", flag->line);
            } else {
                printf("\nLine: %d - Extraneous comma", flag->line);
            }
            flag->firstPass = false;
            return NULL;
        } else {
            return instruction;
        }
    }
}

I *check_i_param(int funcNum, node_t *input, I *instruction, flags *flag, sym_t *symbol, unsigned int IC) {
    bool rs = false, rt = false, immed = false;
    char temp, imm_temp[MAX_LINE_LENGTH] = {0};
    node_t *tempNode = calloc(sizeof(node_t), 1);

    /* -------------------- I think this part is done ------------------ */
    /*
    * Check if the funcNum is one of these 5 Arithmatic and Logic function: addi, subi, andi, ori and nori.
    * Each of them holds 4 fields of data = {opcode, rs, rt, immed} - 2 parameters (rt, rs) -> input in immed
    */

    /*Set the opcode to the function name + the gap to make is match to number in the list */
    instruction->opcode = funcNum + GAP_BETWEEN_LIST_OPCODE;

    if (funcNum >= addi && funcNum <= nori) {
        if (checkNum(input->next, flag)) {
            instruction->immed = atoi(input->next->val); /* get immed at begining because all 3 type of func use it */
            immed = true;

            temp = getReg(input, flag);
            if (temp <= NUM_OF_REG) {
                instruction->rs = temp;
                rs = true;
                input = input->next->next; /* skip immed value */

                temp = getReg(input, flag);
                if (temp <= NUM_OF_REG) {
                    rt = true;
                    input = input->next;
                }
            }
        }
    }
    /*
        * Check if the funcNum is one of these 4 Conditional Directional function : beq, bne, blt, bgt.
        * Each of them holds 4 fields of data = {opcode, rs, rt, immed} - 3 parameters (rt ,rs, immed)
        */

    else if (funcNum >= beq && funcNum <= bgt) {
        /* -------------------- Need to check with you how to get the memory location of the funct name ------------------ */

        /*int distanceFromDestination = 0, locationOfFunc = 0; */

        /* here - get the adress in the memory of the funcName
            * and set it to locationOfFunc
            * locationOfFunc = funcName.adress
            */
        temp = getReg(input, flag);
        if (temp <= NUM_OF_REG) {
            instruction->rs = temp;
            rs = true;
            input = input->next; /* skip immed value */

            temp = getReg(input, flag);
            if (temp <= NUM_OF_REG) {
                instruction->rt = temp;
                rt = true;
                input = input->next;
            }

            strcpy(imm_temp, input->val); /* getting the label ready for parsing (adding ':') */
            strcat(imm_temp, ":");
            tempNode->val = imm_temp;
            if (isLabel(tempNode, flag, symbol)) { /* if operand is label */
                immed = true;
                instruction->immed = IC; /* save current instruction address for later */
                input = input->next;
            }
            /* distanceFromDestination = atoi(input->val);
            instruction->immed = distanceFromDestination - locationOfFunc; */
        }
    }

    /*
    * Check if the funcNum is one of these 6 Save and Load function : lt, sb, lw, sw, lh,sh.
    * Each of them holds 4 fields of data = {opcode, rs, rt, immed} - 3 parameters (rt ,rs, immed)
    * Type of  functions:  
    * in all of these function, the first register contains the adress in the memory
    * the immediate, is th second operand, and he represent the "offset"
    * opcode is the same, and all the rest change the same
    * in this case we need to swip the locations of the "rt" and the "immediate"
    * the "label" is the adress in the memory
    */
    else if (funcNum >= lb && funcNum <= sh) {
        /* -------------------- I think this is also done right? no need to implement further I guess.. ------------------ */
        if (checkNum(input->next, flag)) {
            instruction->immed = atoi(input->next->val); /* get immed at begining because all 3 type of func use it */
            immed = true;

            temp = getReg(input, flag);
            if (temp <= NUM_OF_REG) {
                instruction->rs = temp;
                rs = true;

                input = input->next->next; /* skip immed value */

                instruction->rt = getReg(input, flag);
                if (instruction->rt != NUM_OF_REG) {
                    rt = true;
                    input = input->next;
                }
            }
        }
    }

    /*  check for errors in the input */
    if ((!rs || !rt) && temp <= NUM_OF_REG) {
        printf("\nLine: %d - Missing operand", flag->line);
        flag->firstPass = false;
        return NULL;
    } else if (!immed) {
        printf("\nLine: %d - Missing immediate", flag->line);
        flag->firstPass = false;
        return NULL;
    } else if (input != NULL) {
        if (strlen(input->val) != 0) {
            printf("\nLine: %d - Extraneous operand", flag->line);
        } else {
            printf("\nLine: %d - Extraneous comma", flag->line);
        }
        flag->firstPass = false;
        return NULL;
    }
    return instruction;
}

/*
* This method parses the instruction for J type function.
* It check if all the operands and parameters are valid.
* stop function doesnt accept any opreands, jmp accept registers or label as operands
* la and call only accept labels as operands. 
* if all operands check out, a 32 bit instruction is built based on them
*/
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
            if (input->next != NULL) { /* if there was more thext after stop instruction */
                flag->firstPass = false;
                printf("\nLine: %d - Extraneous operand", flag->line);
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
    if (input->next != NULL) { /* J functions accept only 1 operand, so we check to see if there is more text after current word */
        if (strlen(input->next->val) != 0) {
            printf("\nLine: %d - Extraneous operand", flag->line);
        } else {
            printf("\nLine: %d - Extraneous comma", flag->line);
        }
        flag->firstPass = false;
        free(tempNode);
        return NULL;
    }

    if (instruction->opcode == jmp_opcode) { /* if jmp function, check for register as parameter */
        if (strchr(input->val, '$')) {       /* check if operands is register or label */
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

    strcpy(temp, input->val); /* getting the label ready for parsing (adding ':') */
    strcat(temp, ":");
    tempNode->val = temp;
    if (isLabel(tempNode, flag, symbol)) { /* if operand is label */
        instruction->address = 0;
    }
    free(tempNode);
    return instruction;
}

char getReg(node_t *node, flags *flag) {
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
    * this way, as soon as our input value = registerList[i], we know which register it is by looking at i
    * return register number if found, NUM_OF_REG if register is found but out of range or NUM_OF_REG + 1 if input is not a register
    */
    if (node != NULL && *(node->val) != '\0') {
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
            return NUM_OF_REG + 1;
        }
    }
    return NUM_OF_REG;
}