#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"
#include "global.h"

void assemble(char *fname) {
    unsigned int DC = 0, IC = 100, ICF, DCF;
    unsigned int code_img[1000] = {0};
    dir_t *data_img = calloc(sizeof(dir_t), 1);
    sym_t *symbol_list_head = (sym_t *)calloc(sizeof(sym_t), 1);
    flags *flag = (flags *)malloc(sizeof(flags));

    FILE *fp;
    FILE *f_obj, *f_ent, *f_ext;

    flag->label = false;     /* if a label was found */
    flag->direction = false; /* if its a direction line */
    flag->firstPass = true;  /* if the 1st pass was successful */
    flag->lastLine = false;  /* marks last line in input file */
    flag->external = false;  /* this marks whether to create the .ext fie */
    flag->external = false;  /* this marks whether to create the .ext fie */
    flag->isExt = false;     /* this marks whether current label being processed is external is external */
    flag->line = 1;          /* indicates which line is being processed */

    if ((fp = fopen(fname, "r")) != NULL) {
        printf("Assembling file: %s", fname);
        firstPass(&IC, &ICF, &DC, &DCF, code_img, data_img, symbol_list_head, flag, fp);
        secondPass(ICF, DCF, symbol_list_head, flag, fp);

        /* creating oputput files */
        if (flag->firstPass && flag->secondPass) {
            if ((f_obj = createFile(fname, ".ob")) != NULL) {
                printObj(f_obj, code_img, data_img, ICF, DCF);
            }

            if (flag->external) { /* .ext file */
                if ((f_ext = createFile(fname, ".ext")) != NULL) {
                    printExt();
                }
            }
            if (flag->entry) { /* .ent file */
                if ((f_ent = createFile(fname, ".ent")) != NULL) {
                    printEnt(f_ent, symbol_list_head);
                }
            }
            printf("\nAssembly completed.");
        }

    } /* second pass end */
    if (!flag->firstPass || !flag->secondPass) {
        printf("\nErrors were detected. No output files were created.");
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
                printf("Illegal file name or wrong file extention. file extentions should only be \".as\"\n");
            }
        }
    } else {
        printf("No files were set for processing\n");
    }
    return true;
}