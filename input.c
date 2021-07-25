#include "input.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
node_t* initList();

unsigned int instruction = 0;
/* 32 bit command 
000000 00000 00000 00000 00000 000000
opcd     rs   rt     rd   fnct  immed   
*/

char* savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

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

node_t* getLine(FILE* fp, flags* flag) { /* saves each word a new node */

    node_t* head = initList();
    node_t* node = head;
    char comma = false;
    char temp = fgetc(fp);
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
                temp = fgetc(fp);
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
            temp = fgetc(fp);
            continue;
        } else {
            comma = false;
            *(currVal + j) = temp;
            j++;
            prevChar = temp;
            temp = fgetc(fp);
        }
    }
    return head;
}

char checkIfLabel(node_t* input, flags* flag) {
    int i;
    char* temp = input->val;
    if (strstr(temp, ":") != NULL) {
        temp[strlen(temp) - 1] = '\0';
        for (i = 0; i < 33; i++) { /* 33 is number of saved words*/
            if (strcmp(savedWords[i], input->val) == 0) {
                return true;
            }
        }
    }
    flag->error = true;
    printf("\nLine: %d - illigal label name", flag->line);
    return false;
}
