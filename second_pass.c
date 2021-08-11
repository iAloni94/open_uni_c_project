#include <string.h>

#include "global.h"

void secondPass(unsigned int ICF, unsigned int DCF, sym_t *symbol_list_head, flags *flag, FILE *fp) {
      char *functionName[NUM_OF_FUNC] = {
        "add", "sub", "and", "or",
        "nor", "move", "mvhi", "mvlo",
        "addi", "subi", "andi", "ori",
        "nori", "bne", "beq", "blt",
        "bgt", "lb", "sb", "lw",
        "sw", "lh", "sh", "jmp",
        "la", "call", "stop"};

    char *directives[NUM_OF_DIR] = {".db", ".dh", ".dw", ".asciz", ".extern", ".entry"};

    int funcNum, dirNum, i;
    node_t *head, *node;

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
                if (isLabel(node, flag, symbol_list_head)) { /* skip if label is found */
                    node = node->next;
                }

                dirNum = NUM_OF_DIR;
                for (i = 0; i < NUM_OF_DIR; i++) {
                    if (!strcmp(node->val, directives[i])) {
                        dirNum = i;
                    }
                }

                if (dirNum == ent) { /* handle entry directive */
                    ent_handler(symbol_list_head, node, flag);
                    freeInputList(head);
                    flag->line += 1;
                    continue;
                } else {
                    freeInputList(head);
                    flag->line += 1;
                    continue;
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
        } /* end while */
    }
}