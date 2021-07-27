#include "input.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"

node_t* initList();

unsigned int instruction = 0;
/* 32 bit command 
000000 00000 00000 00000 00000 000000
opcd     rs   rt     rd   fnct  immed   
*/

node_t* initList() {
    node_t* head = malloc(sizeof(node_t));
    head->val = calloc(1, 100);
    head->next = NULL;
    return head;
}

node_t* addNode() {
    node_t* newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, 100);
    newNode->next = NULL;
    return newNode;
}

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
                flag->error = true;
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

char checkIfLabel(node_t* input) {
    char* temp = input->val;
    if (strstr(temp, ":") != NULL) {
        input->val[strlen(temp) - 1] = '\0';
        if (strlen(temp) <= LABEL_MAX_LENGTH && IS_LETTER(*temp) && isAlphaNumeric(temp) && !isReserved(temp)) return true;
    }
    printf("\nLine: %d - Illigal label name", 1);
    return false;
}
