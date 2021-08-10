#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"
#include "global.h"

void assemble(char *fname) {
    char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    char *directions[NUM_OF_DIR] = {".db", ".dh", ".dw", ".asciz", ".extern", ".entry"};

    unsigned int DC = 0, IC = 100, ICF, DCF;
    int funcNum, dirNum, i, codeCounter = 0;
    /* char data_img[1000] = {0}; */
    unsigned int code_img[1000] = {0};
    dir_t *this_data, *data_img = calloc(sizeof(dir_t), 1);
    sym_t *symbol_list_head = (sym_t *)calloc(sizeof(sym_t), 1);
    flags *flag = (flags *)malloc(sizeof(flags));
    node_t *head, *node;
    unsigned int first_pass_32bit = 0;
    FILE *fp;
    FILE *f_obj, *f_ent, *f_ext;

    fp = fopen(fname, "r");

    flag->label = false;     /* if a label was found */
    flag->direction = false; /* if its a direction line */
    flag->firstPass = true;  /* if the 1st pass was successful */
    flag->lastLine = false;  /* marks last line in input file */
    flag->external = false;  /* this marks whether to create the .ext fie */
    flag->external = false;  /* this marks whether to create the .ext fie */
    flag->line = 1;          /* indicates which line is being processed */

    printf("Assembling file: %s", fname);

    if (flag == NULL || symbol_list_head == NULL || data_img == NULL) {
        printf("Memory allocation error");
        exit(0);
    }

    if (fp) {
        this_data = data_img;
        while (!(flag->lastLine)) {   /* first pass */
            head = getLine(fp, flag); /* each node in input list contains a single word */
            if (head == NULL) {
                flag->line += 1;
            } else {
                node = head;

                if ((flag->label = isLabel(node, flag, symbol_list_head))) { /* raise flag if label found */
                    if (isDeclared(node->val, symbol_list_head, flag))
                        flag->label = false; /* check if label was already declared*/
                    node = node->next;
                }

                dirNum = NUM_OF_DIR;
                for (i = 0; i < NUM_OF_DIR; i++) {
                    if (!strcmp(node->val, directions[i])) {
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
                        R *instruction = (R *)calloc(sizeof(R), sizeof(char));
                        if (instruction == NULL) {
                            flag->firstPass = false;
                            printf("\nMemory allocation error");
                            exit(0);
                        }
                        if ((instruction = check_r_param(funcNum, node->next, instruction, flag))) {
                            first_pass_32bit = r_binary_instruction(instruction);
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
                            free(instruction);
                        }
                    } else if (funcNum == NUM_OF_FUNC) { /* undefined function */
                        flag->firstPass = false;
                        printf("\nLine: %d - Unrecognized instruction \"%s\"", flag->line, node->val);
                        freeInputList(head);
                        flag->line += 1;
                        IC += 4;
                        continue;
                    }

                    if (flag->label) { /* if found, inserts label into symbol table. each node is a label */
                        insertLabel(symbol_list_head, head, flag, IC, DC);
                    }
                    freeInputList(head);
                    code_img[codeCounter] = first_pass_32bit; /* insert binary instruction to memory image */
                    codeCounter++;
                    IC += 4;
                    flag->line++;
                } else {               /* if directive */
                    if (flag->label) { /* if label is found, inserts label into symbol table. each node is a label */
                        insertLabel(symbol_list_head, head, flag, IC, DC);
                    }
                    switch (dirNum) {
                        case db:
                            this_data = save_byte(node->next, this_data, &DC, flag);
                            break;
                        case dh:
                            this_data = save_half_word(node->next, this_data, &DC, flag);
                            break;
                        case dw:
                            this_data = save_word(node->next, this_data, &DC, flag);
                            break;
                        case asciz:
                            this_data = save_str(node->next, this_data, &DC, flag);
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
        ICF = IC;
        DCF = DC;
        if (symbol_list_head->name != NULL) {
            updateSymbolAddress(symbol_list_head, ICF);
        }
    } else {
        printf("Failed to open file. Trying next file.");
        return;
    } /* first pass end*/

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
                /* step 3 in second pass algorithem  */
                if (isLabel(node, flag, symbol_list_head)) { /* skip if label is found */
                    node = node->next;
                }

                for (i = 0; i < NUM_OF_DIR - 1; i++) { /* step 4  */
                    if (!strcmp(node->val, directions[i])) {
                        flag->line += 1;
                        continue;
                    }
                }

                if (!strcmp(node->val, ".entry")) { /* step 5 */
                    flag->entry = true;
                    node = node->next;
                    if (isDeclared(node->val, symbol_list_head, flag)) { /* step 6 */
                        sym_t *tempSym = symbol_list_head;
                        while (tempSym != NULL) {
                            if (!strcmp(node->val, tempSym->name)) {
                                tempSym->attribute = "entry";
                                flag->entry = true;
                                flag->line += 1;
                                break;
                            }
                            tempSym = tempSym->next;
                        }
                    } else {
                        printf("\nLine: %d - Label was not declared", flag->line);
                        flag->entry = false;
                        flag->secondPass = false;
                        flag->line += 1;
                        freeInputList(head);
                        continue;
                    }
                }

                funcNum = NUM_OF_FUNC;
                for (i = 0; i < NUM_OF_FUNC; i++) { /* step 7 */
                    if (!strcmp(node->val, functionName[i])) {
                        funcNum = i;
                        break;
                    }
                }
            }
            freeInputList(head);
            flag->line += 1;
        }

        if (flag->secondPass) { /* step 9 */
            /* creating oputput files          step 10  */
            f_obj = createFile(fname, ".ob"); /* .obj file */
            printObj(f_obj, code_img, data_img, ICF, DCF);

            if (flag->external) { /* .ext file */
                f_ext = createFile(fname, ".ext");
                printExt();
            }
            if (flag->entry) { /* .ent file */
                f_ent = createFile(fname, ".ent");
                printEnt();
            }
            printf("\nAssembly completed.");
        }

    } /* second pass end */
    else {
        printf("\nErrors were detected. aborting assembly process.");
    }

    freeMemory(flag, symbol_list_head, data_img, fp, f_obj, f_ext, f_ent); /* Closing files and clearing memory before ending assembly process */
    printf("\n");
    return;
}

int main(int argc, char *argv[]) {
    int i;
    char *ext;
    if (argc > 1) {
        for (i = 1; i < argc; ++i) {
            ext = strchr(argv[i], '.');
            if (!strcmp(ext, FILE_EXT)) {
                assemble(argv[i]);
            } else {
                printf("Error! File: %s - Input file extentions should only be \".as\"\n", argv[i]);
            }
        }
    } else {
        printf("No files were set for processing\n");
    }
    return true;
}