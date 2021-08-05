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

    void (*functions[NUM_OF_FUNC])() = {
        add_func, sub_func, and_func, or_func,
        nor_func, move_func, mvhi_func, mvlo_func,
        addi_func, subi_func, andi_func, ori_func,
        nori_func, bne_func, beq_func, blt_func,
        bgt_func, lb_func, sb_func, lw_func,
        sw_func, lh_func, sh_func, jmp_func,
        la_func, call_func, stop_func};

    char *directions[NUM_OF_DIR] = {".dh", ".dw", ".db", ".asciz", ".extern", ".entry"};

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
    flag->lastLine = false;  /* marks last line in input file */
    flag->external = false;  /* this marks whether to create the .ext fie */
    flag->entry = false;     /* this marks whether we need the .ent file */
    flag->line = 1;          /* indicates which line is being proccessed */

    symbol = symbol_list_head;

    printf("Assembling file: %s", fname);

    if (symbol_list_head == NULL || flag == NULL) {
        printf("Memory allocation error");
        exit(0);
    }

    for (i = 0; i < NUM_OF_REG; i++) { /* registers init - regArray contains pointers to all registers 0-31 */
        regArray[i] = (reg_ptr)calloc(sizeof(reg_t), 1);
        if (regArray[i] == NULL) {
            printf("Memory allocation error");
            exit(0);
        }
    }

    if (fp) {
        while (!(flag->lastLine)) {
            head = getLine(fp, flag); /* each node in input list contains a single word */

            if (head == NULL) {
                flag->line += 1;
                IC += 4;
            } else {
                node = head;
                if (isColon(node, flag)) {
                    if ((flag->label = isLabel(node, flag, symbol_list_head))) {      /* raise flag for label. */
                        if (isDeclared(node->val, symbol, flag)) flag->label = false; /* check if label was already declared*/
                    }
                    node = node->next;
                }

                for (i = 0; i < NUM_OF_DIR; i++) {
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
                        exit(0);
                    }
                    if ((instruction = check_r_param(funcNum, node->next, instruction, flag))) {
                        first_pass_32bit = r_binary_instruction(instruction);
                        functions[funcNum](instruction);
                        free(instruction);
                    }
                } else if (funcNum <= sh) { /* I type function */
                    I *instruction = (I *)calloc(sizeof(I), sizeof(char));
                    if (instruction == NULL) {
                        flag->firstPass = false;
                        printf("\nMemory allocation error");
                        exit(0);
                    }
                    if ((instruction = check_i_param(funcNum, node->next, instruction, flag))) {
                        first_pass_32bit = i_binary_instruction(instruction);
                        functions[funcNum](instruction);
                        free(instruction);
                    }
                } else if (funcNum <= stop) { /* J type function */
                    J *instruction = (J *)calloc(sizeof(J), sizeof(char));
                    if (instruction == NULL) {
                        flag->firstPass = false;
                        printf("\nMemory allocation error");
                        exit(0);
                    }
                    if ((instruction = check_j_param(funcNum, node, instruction, flag, symbol_list_head))) {
                        first_pass_32bit = j_binary_instruction(instruction);
                        if (instruction->opcode == 32) {
                            functions[funcNum](instruction, IC);
                        } else {
                            functions[funcNum](instruction);
                        }
                        free(instruction);
                    }
                } else if (funcNum == NUM_OF_FUNC && flag->direction == false) { /* undefined function */
                    flag->firstPass = false;
                    printf("\nLine: %d - Unrecognized instruction <%s>", flag->line, node->val);
                }

                if (flag->label) { /* if found, inserts label into symbol table. each node is a label */
                    symbol->name = calloc(sizeof(char), strlen(head->val));
                    memcpy(symbol->name, head->val, strlen(head->val));
                    if (flag->direction) {
                        symbol->address = DC;
                        symbol->attribute = "data";
                    } else {
                        symbol->address = IC;
                        symbol->attribute = "code";
                    }
                    symbol->next = calloc(sizeof(sym_t), 1);
                    symbol = symbol->next;
                    flag->label = false;
                }

                code_img[j] = first_pass_32bit; /* insert binary instruction to memory image */
                freeInputList(head);
                flag->line += 1;
                flag->direction = false;
                IC += 4;
                j++;
            }
        } /* while loop */
    } else {
        printf("Failed to open file. Trying next file.");
        return false;
    }

    ICF = IC;
    DCF = DC;

    if (flag->firstPass) {
        /* second pass things - updating symbol list, printing file, freeing memory, closing file*/
        FILE *f_obj, *f_ent, *f_ext;

        /* creating oputput files */
        f_obj = createFile(fname, ".ob"); /* hex machine code */
        if (flag->external) {
            f_ext = createFile(fname, ".ext");
        }
        if (flag->entry) {
            f_ent = createFile(fname, ".ent");
        }

        /* Closing files and clearing memory before ending assembly proccess */
        fclose(f_obj);

        if (flag->external) {
            fclose(f_ext);
        }
        if (flag->entry) {
            fclose(f_ent);
        }
        printf("\nAssembly completed without errors.\n");
    }
    fclose(fp);
    freeSymbolTable(symbol_list_head);
    free(flag);
    for (i = 0; i < NUM_OF_REG; i++) {
        free(regArray[i]);
    }
    return true;
}

int main(int argc, char *argv[]) {
    int i;
    char *ext;
    for (i = 1; i < argc; ++i) {
        ext = strchr(argv[i], '.');
        if (!strcmp(ext, FILE_EXT)) {
            assemble(argv[i]);
        } else {
            printf("\nError! File: %s - Input file extentions should only be \".as\"\n", argv[i]);
        }
    }
    return 0;
}