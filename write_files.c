#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "directive.h"
#include "label.h"
#include "utils.h"

#define MASK 0xFF /* 00000000 00000000 00000000 11111111 */

/*
* In order to print the output files, first we will get the fine name without the ".as" extention
* That way a file with the same name and a different extention (.obj, .ent, .ext) can be created
*/
FILE* createFile(char* fname, char* extention) {
    int i;
    char ext_diff = strlen(fname) - 3; /* length of ".as" = 3 */
    char* fname_no_extention = calloc(sizeof(char), ext_diff);
    FILE* output_file_ptr;

    for (i = 0; i < (ext_diff); i++) {
        *(fname_no_extention + i) = *(fname + i);
    }
    /* Opening file for output */
    output_file_ptr = fopen(strcat(fname_no_extention, extention), "w");
    free(fname_no_extention);
    return output_file_ptr;
}


/* This function print the .obj file according to the specifications */
void printObj(FILE* fp, unsigned int* codeImg, dir_t* dataImg, unsigned int ICF, unsigned int DCF) {
    unsigned int a, b, c, d, i = 0, j = 0, IC = 100;
    unsigned char* data = malloc(DCF+1);

    fprintf(fp, "\t%d\t%d", ICF - 100, DCF);

    /*  print code image*/
    /* shift mask to desired 4 bits location and then shift the result to the rightmost 4 bits for printing */
    for (i = 0; *(codeImg + i) != 0; i++, IC += 4) {
        a = *(codeImg + i) & MASK;                 /* instruction: bytes 0-8; mask:   00000000 00000000 00000000 11111111 */
        b = (*(codeImg + i) & (MASK << 8)) >> 8;   /* instruction: bytes 9-16; mask:  00000000 00000000 11111111 00000000 */
        c = (*(codeImg + i) & (MASK << 16)) >> 16; /* instruction: bytes 17-24; mask: 00000000 11111111 00000000 00000000 */
        d = (*(codeImg + i) & (MASK << 24)) >> 24; /* instruction: bytes 25-32; mask: 11111111 00000000 00000000 00000000 */

        fprintf(fp, "\n%04d  %02X %02X %02X %02X", IC, a, b, c, d);
        
    }

    /* print data image */
    /* fills data array with data for printing, each cell is 4 bits for easier printing */
    i = 0;
    while (dataImg->next != NULL) {
        switch (dataImg->flag) {
            case str:
            case byte:
                *(data + i) = dataImg->byte;
                break;
            case half_word:
                *(data + i++) = dataImg->half_word;
                *(data + i) = (dataImg->half_word) >> 8;
                break;
            case word:
                *(data + i++) = dataImg->word;
                *(data + i++) = (dataImg->word) >> 8;
                *(data + i++) = (dataImg->word) >> 16;
                *(data + i) = (dataImg->word) >> 24;
                break;
            default:
                break;
        }
        dataImg = dataImg->next;
        i++;
    }

    for (i = 0; i < DCF; i += 4, j += 4) {
        fprintf(fp, "\n%04d  ", ICF + j);
        fprintf(fp, "%02X %02X %02X %02X", data[i], data[i + 1], data[i + 2], data[i + 3]);
        
    }
    free(data);
}

void printExt(FILE *fp, ext_t *ext_node) {
    while(ext_node->next != NULL){
        fprintf(fp, "%s %04d\n", ext_node->name, ext_node->address);
        ext_node = ext_node->next;
    }
}

void printEnt(FILE* fp, sym_t* symbol) {
    while (symbol != NULL) {
        if (strstr(symbol->attribute, "entry")) {
            fprintf(fp, "%s %04d\n", symbol->name, symbol->address);
        }
        symbol = symbol->next;
    }
}

void writeFiles(char* fname, unsigned int* code_img, dir_t* data_img, sym_t* symbol_list_head, flags* flag, ext_t *ext_list_head, unsigned int ICF, unsigned int DCF) {
    FILE *f_obj, *f_ent, *f_ext;

    if ((f_obj = createFile(fname, ".ob")) != NULL) {
        printObj(f_obj, code_img, data_img, ICF, DCF);
        fclose(f_obj);
    }

    if (flag->external) { /* .ext file */
        if ((f_ext = createFile(fname, ".ext")) != NULL) {
            printExt(f_ext, ext_list_head);
            fclose(f_ext);
        }
    }
    if (flag->entry) { /* .ent file */
        if ((f_ent = createFile(fname, ".ent")) != NULL) {
            printEnt(f_ent, symbol_list_head);
            fclose(f_ent);
        }
    }
}
