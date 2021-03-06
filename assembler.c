#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"
#include "global.h"

void assemble(char *fname) {
    unsigned int DC = 0, IC = INITIAL_MEM_ADDRESS, ICF, DCF;
    unsigned int code_img[CODE_MEM_SIZE] = {0};
    dir_t *data_img = calloc(sizeof(dir_t), 1);
    sym_t *symbol_list_head = (sym_t *)calloc(sizeof(sym_t), 1);
    flags *flag = (flags *)calloc(sizeof(flags), 1);
    ext_t *ext_list_head = (ext_t *)calloc(sizeof(ext_t), 1);

    FILE *fp;

    flag->label = false;
    flag->direction = false;
    flag->firstPass = true;
    flag->lastLine = false;
    flag->external = false;
    flag->entry = false;
    flag->isExt = false;
    flag->line = 1;

    if ((fp = fopen(fname, "r")) != NULL) {
        printf("Assembling file: %s", fname);
        firstPass(&IC, &ICF, &DC, &DCF, code_img, data_img, symbol_list_head, flag, fp);
        if (flag->firstPass) secondPass(ICF, DCF, code_img, symbol_list_head, flag, fp, ext_list_head);

        /* creating oputput files */
        if (flag->firstPass && flag->secondPass) {
            writeFiles(fname, code_img, data_img, symbol_list_head, flag, ext_list_head, ICF, DCF);

            printf("\nAssembly completed.");
        }
        if (!flag->firstPass || !flag->secondPass) {
            printf("\nErrors were detected. No output files were created.\n");
        }
    } else {
        printf("\nFailed to open file. Trying next file.");
    }
    freeMemory(flag, symbol_list_head, data_img, ext_list_head, fp); /* Closing files and clearing memory before ending assembly process */
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