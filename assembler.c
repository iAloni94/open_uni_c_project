#include "assembler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"
#include "param.h"
#include "utils.h"

int assemble(char *fname) {
    char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    void (*functions[NUM_OF_FUNC + 1])() = {
        add_func, sub_func, and_func, or_func,
        nor_func, move_func, mvhi_func, mvlo_func,
        addi_func, subi_func, andi_func, ori_func,
        nori_func, bne_func, beq_func, blt_func,
        bgt_func, lb_func, sb_func, lw_func,
        sw_func, lh_func, sh_func, jmp_func,
        la_func, call_func, stop_func, undef_func};

    char *directions[NUM_OF_DIR] = {".dh", ".dw", ".db", "asciz", ".extern", ".entry"};

    unsigned int DC = 0, IC = 100, ICF, DCF;
    int funcNum, i, j = 0;
    long data_img[1000];
    long code_img[1000];
    sym_t *symbol, *symbol_list_head = calloc(sizeof(sym_t), 1);
    flags *flag = (flags *)malloc(sizeof(flags));
    node_t *head, *node;
    unsigned int first_pass_32bit = 0;
    FILE *fp;

    fp = fopen(fname, "r");

    flag->label = false;     /* if a label was found */
    flag->direction = false; /* if its a direction line */
    flag->firstPass = true;  /* if the 1st pass was successful */
    flag->lastLine = false;
    flag->line = 1; /* indicates which line is being proccessed */

    symbol = symbol_list_head;

    printf("Assembling file: %s\n", fname);

    if (symbol_list_head == NULL || flag == NULL) {
        printf("Memory allocation error");
        return false;
    }

    for (i = 0; i < NUM_OF_REG; i++) { /* registers init - regArray contains pointers to all registers 0-31 */
        regArray[i] = (reg_ptr)calloc(sizeof(reg_t), 1);
        if (regArray[i] == NULL) {
            printf("Memory allocation error");
            return false;
        }
    }

    if (fp) {
        while (!(flag->lastLine)) {
            head = getLine(fp, flag);
            node = head;
            if (head) {
                if ((flag->label = isLabel(node, flag))) {
                    isDeclared(node, symbol_list_head, flag);
                    node = node->next;
                }

                for (i = 0; i < NUM_OF_DIR + 1; i++) {
                    if (!strcmp(node->val, directions[i])) {
                        flag->direction = true;
                        /* handle directions - directions.c */
                        break;
                    }
                }

                funcNum = NUM_OF_FUNC;
                for (i = 0; i < NUM_OF_FUNC; i++) {
                    if (!strcmp(node->val, functionName[i])) {
                        funcNum = i;
                        break;
                    }
                }

                if (funcNum <= mvlo) { /* R type function */
                    R *instruction = (R *)calloc(sizeof(R), sizeof(char));
                    if (instruction == NULL) {
                        flag->firstPass = false;
                        printf("\nMemory allocation error");
                        return false;
                    }
                    if ((instruction = check_r_param(funcNum, node->next, instruction, flag))) {
                        first_pass_32bit = r_binary_instruction(instruction);
                        functions[funcNum](instruction);
                    }
                } else if (funcNum <= sh) { /* I type function */
                    I *instruction = (I *)calloc(sizeof(I), sizeof(char));
                    if (instruction == NULL) {
                        flag->firstPass = false;
                        printf("\nMemory allocation error");
                        return false;
                    }
                    if ((instruction = check_i_param(funcNum, node->next, instruction, flag))) {
                        first_pass_32bit = i_binary_instruction(instruction);
                        functions[funcNum](instruction);
                    }
                } else if (funcNum <= stop) { /* J type function */
                    J *instruction = (J *)calloc(sizeof(J), sizeof(char));
                    if (instruction == NULL) {
                        flag->firstPass = false;
                        printf("\nMemory allocation error");
                        return false;
                    }
                    if ((instruction = check_j_param(funcNum, node, instruction, flag, symbol_list_head))) {
                        first_pass_32bit = j_binary_instruction(instruction);
                        if (instruction->opcode == 32) {
                            functions[funcNum](instruction, IC);
                        } else {
                            functions[funcNum](instruction);
                        }
                    }
                } else if (funcNum == NUM_OF_FUNC && isAlphaNumeric(head->val)) { /* undefined function */
                    flag->firstPass = false;
                    printf("\nLine: %d - Unrecognized instruction <%s>", flag->line, node->val);
                }

                if (flag->label) { /* if found, inserts label into symbol table. each node is a label */
                    symbol->name = head->val;
                    if (flag->direction) {
                        symbol->address = DC;
                        symbol->attribute = "code";
                    } else {
                        symbol->address = IC;
                        symbol->attribute = "data";
                    }
                    flag->label = false;
                    symbol->next = calloc(sizeof(sym_t), 1);
                    symbol = symbol->next;
                }

                code_img[j] = first_pass_32bit; /* insert binary instruction to memory image */
                freeInputList(head);
                flag->line += 1;
                IC += 4;
                j++;
                head = getLine(fp, flag);
            } else { /* if there was an input problen, advance IC and proccess next line */
                IC += 4;
            }
        }
    } else {
        printf("Failed to open file. Trying next file.");
        return false;
    }

    if (flag->firstPass) {
        ICF = IC;
        DCF = DC;
        /* second pass things - updating symbol list, printing file, freeing memory, closing file*/

        freeSymbolTable(symbol_list_head);
    }

    printf("\n");
    return 1;
}

int main(int argc, char *argv[]) {
    int i;
    char assembled = true;
    char *ext;
    for (i = 1; i < argc; ++i) {
        if (!assembled) puts("");
        ext = strchr(argv[i], '.');
        if (!strcmp(ext, FILE_EXT)) {
            assembled = assemble(argv[i]);
        } else {
            printf("\nError! File: %s - Input file extentions should only be \".as\"\n", argv[i]);
        }
    }
    return 0;
}