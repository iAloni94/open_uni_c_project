#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"


node_t* getLine(char* line, flags* flag) { /* saves each word a new node */
    int i = 0;
    node_t* head = initList();
    node_t* node = head;
    char comma = false;
    char temp = *(line);
    char prevChar = temp;
    int j = 0;
    char* currVal = node->val;

    while (temp != '\n') {
        if (temp == ';') {
            return NULL;
        }
        if (temp == EOF) {
            node->next = addNode();
            node = node->next;
            *(node->val) = temp;
            return head;
        }
        if (isspace(temp)) {
            while (isspace(temp) && temp != '\n') {
                prevChar = temp;
                i++;
                temp = *(line + i);
            }
            if (comma && temp == ',') {
                comma = false;
                flag->firstPass = false;
                printf("\nLine: %d - consecutive commas", flag->line);
            } else if ((isspace(prevChar) || temp == ',') && *currVal != -1) {
                node->next = addNode();
                node = node->next;
                currVal = node->val;
                *(currVal) = -1;
                j = 0;
                prevChar = temp;
            }
            continue;
        } else if (temp == ',') {
            comma = true;
            prevChar = temp;
            i++;
            temp = *(line + i);
            continue;
        } else {
            comma = false;
            *(currVal + j) = temp;
            j++;
            i++;
            prevChar = temp;
            temp = *(line + i);
        }
    }
    return head;
}

char isLabel(node_t* input, flags *flag) {
    char* temp = input->val;
    if (strstr(temp, ":") != NULL) {
        input->val[strlen(temp) - 1] = '\0';
        if (strlen(temp) <= LABEL_MAX_LENGTH && IS_LETTER(*temp) && isAlphaNumeric(temp) && !isReserved(temp)) return true;
    }
    flag->firstPass = false;
    printf("\nLine: %d - Illigal label name", 1);
    return false;
}
