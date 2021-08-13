
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

#define MASK_1_BTYE 0xFFFF /* ..00 0000 1111 1111 1111 1111 */

void freeDataImg(dir_t *data) {
    while (data != NULL) {
        dir_t *temp = data;
        data = data->next;
        free(temp);
    }
}

void freeExtList(ext_t *extNode) {
    while (extNode != NULL) {
        ext_t *temp = extNode;
        extNode = extNode->next;
        free(temp);
    }
}

void freeMemory(flags *flag, sym_t *symbol, dir_t *data_img, ext_t *ext_head, FILE *fp) {
    if (fp) fclose(fp);
    freeDataImg(data_img);
    freeSymbolTable(symbol);
    freeExtList(ext_head);
    free(flag);
}

void calcDistance(unsigned int *instruction_address, unsigned int label_address) {
    int dist;
    unsigned int curr_address = *instruction_address & MASK_1_BTYE; /* get the address of the instruction itself (rightmost 16 bits) */
    dist = label_address - curr_address;                            /* new address to be entered to code image = the distance between the two */
    dist = dist & MASK_1_BTYE;                                      /* get rightmost 16 bits of new address */
    *instruction_address = *instruction_address & ~MASK_1_BTYE;     /* turn off bit 0-16 of original machine code */
    *instruction_address = *instruction_address | dist;
}