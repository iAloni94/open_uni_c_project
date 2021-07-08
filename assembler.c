#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

int main(int argc, char *argv[]) {
    /*
    char *functionName[27] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    int (*functions[28])() = {
                                add_func, sub_func, and_func, or_func,
                                nor_func, move_func, mvhi_func, mvlo_func,
                                addi_func, subi_func, andi_func, ori_func,
                                nori_func, bne_func, beq_func, blt_func,
                                bgt_func, lb_func, sb_func, lw_func,
                                sw_func, lh_func, sh_func, jmp_func,
                                la_func, call_func, stop_func, undef_func};
*/
    if (argc <= 1) {
        printf("No files were detected");
    } else {
        FILE *fp;
        fp = fopen(argv[1], "r");
        if (fp) {
            getLine(fp);
        }
    }

    return 1;
}