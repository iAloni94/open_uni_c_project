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

        if (!rs || !rd || !rt) {
            printf("\nLine: %d - Missing operands", flag->line);
            flag->firstPass = false;
            return NULL;
        } else if (input != NULL) {
            if (strlen(input->val) != 0) {
                printf("\nLine: %d - extraneous operand", flag->line);
            } else {
                printf("\nLine: %d - extraneous comma", flag->line);
            }
            flag->firstPass = false;
            return NULL;
        }
        return instruction;
    }
    else { /* copy functions - 2 parameters rs + rd */
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
    char rs = false, rt = false, immed = false;
    int i = 0;

    if (funcNum >= addi && funcNum <= nori) {
        /*Set the opcode to the function name + the gap to make is similar to number in the list */
        instruction->opcode = funcNum + GAP_BETWEEN_LIST_OPCODE;

        /* getting the rs value from register */
        if (instruction->rs >= NUM_OF_REG || instruction->rs >= NUM_OF_REG) {
            instruction->rs = getReg(input->val, flag);
            rs = true;
            input = input->next;
            char *rtAdress = input;
        }
        instruction->immed = input->val;
        input = input->next;

        instruction->rt = getReg(input->val, flag);
        if (instruction->rt != NUM_OF_REG) {
            rt = true;
        }
    }

    /* while (i < strlen(input->next->val))
     {
         if (!IS_NUM(*(input->next->val + i)))
         {
             flag->firstPass = false;
             printf("\nLine: %d - Illegal parameter. Immed value should only be an integer", flag->line);
             return NULL;
         }
         i++;
     }
     instruction->immed = atoi(input->next->val);
*/
    /*
    * Check if the funcNum is one of these 5 Arithmatic and Logic function: addi, subi, andi, ori and nori.
    * Each of them holds 4 fields of data = {opcode, rs, rt, immed} 
    *  */

    else if (funcNum >= beq && funcNum <= bgt) {
        /*
        * Check if the funcNum is one of these 4 Conditional Directional function: beq, bne, blt, bgt.  
        * each function we need to check the value that is inside the register (binary / decimal?) */
        /* the distance is from the current location of the instruction minus the current value in the loop. 
        if the loop adress is smaller than the adress of the instruction 
        (declared before it{in this case I am not sure what is the purpose of minus}) 
        /* bne - check if the value in register "rs" is -- Not Equals-- to the value in register "rt" */
        /* blt - check if the value in register "rs" is -- Lower Than -- the value in register "rt" */
        /* bgt - check if the value in register "rs" is -- Greater Than -- to the value in register "rt" */

        /*Set the opcode to the function name + the gap to make is similar to number in the list */

        instruction->opcode = funcNum + GAP_BETWEEN_LIST_OPCODE;

        /* getting the rs value from register */
        instruction->rs = getReg(input->val, flag);
        if (instruction->rs != NUM_OF_REG) {
            rs = true;
            input = input->next;
            /* char *rtAdress = input; */
        }

        instruction->rt = getReg(input->val, flag);
        if (instruction->rt != NUM_OF_REG) {
            rt = true;
            input = input->next;
        }

        instruction->immed = input->val;
    }

    else if (funcNum >= lb && funcNum <= sh) { /* Type of save and load functions:
    lb - load byte - load byte from the memory
    sb - save byte - save byte in the memory
    lw - load word - load word from the memory
    sw - save word - save word in the memory
    lh - load half word - load half word from the memory
    sh - save half word - save half word in the memory
    
    in all of these function, the first register contains the adress in the memory
    the immediate, is th second operand, and he represent the "offset"
    opcode is the same, and all the rest change the same
    in this case we need to swip the locations of the "rt" and the "immediate"
    the "label" is the adress in the memory
    */

        instruction->opcode = funcNum + GAP_BETWEEN_LIST_OPCODE;

        /* getting the rs value from register */
        instruction->rs = getReg(input->val, flag);
        if (instruction->rs != NUM_OF_REG) {
            rs = true;
            input = input->next;
            /* char *rtAdress = input; */
        }
        instruction->immed = input->val;
        input = input->next;

        instruction->rt = getReg(input->val, flag);
        if (instruction->rt != NUM_OF_REG) {
            rt = true;
            input = input->next;
        }
    }

    if (funcNum <= addi && funcNum <= sh && (rs && rt && immed)) {
        return instruction;
    } else if (rs && rt) {
        return instruction;
    } else {
        flag->firstPass = false;
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

    strcpy(temp, input->val); /* getting the label ready for parsing (adding ':') */
    strcat(temp, ":");
    tempNode->val = temp;
    if (isLabel(tempNode, flag, symbol)) { /* if operand is label */
        instruction->address = 0;
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
    * this way, as soon as our input value = registerList[i], we know which register it is by looking at i
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
        }
    }
    return NUM_OF_REG;
}