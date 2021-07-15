#include "input.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node_t* initList();

unsigned int instruction = 0;
/* 32 bit command 
000000 00000 00000 00000 00000 000000
opcd     rs   rt     rd   fnct  immed   
*/

char* registerList[] = {
    "$0", "$1", "$2", "$3",
    "$4", "$5", "$6", "$7",
    "$8", "$9", "$10", "$11",
    "$12", "$13", "$14", "$15",
    "$16", "$17", "$18", "$19",
    "$20", "$21", "$22", "$23",
    "$24", "$25", "$26", "$27",
    "$28", "$29", "$30", "$31",
    "$32"};

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

node_t* getLine(FILE* fp) { /* saves each word a new node */

    node_t* head = initList();
    node_t* node = head;
    char comma = false;
    char temp = fgetc(fp);
    char prevChar = temp;
    int j = 0;
    char* currVal = node->val;

    while (temp != '\n') {
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
            if ((isspace(prevChar) || temp == ',') && *currVal != -1) {
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
            if (comma && prevChar == ',') {
                printf("EROR!!! consecutive commas");
                comma = false;
            }
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

char* savedWords[] = {
    "dd", "dw", "db", "asciz", "entry", "extern",
    "add", "sub", "and", "or",
    "nor", "move", "mvhi", "mvlo",
    "addi", "subi", "andi", "ori",
    "nori", "bne", "beq", "blt",
    "bgt", "lb", "sb", "lw",
    "sw", "lh", "sh", "jmp",
    "la", "call", "stop"};

char checkIfLabel(node_t* input) {
    int i;
    char* temp = input->val;
    while (temp != NULL) {
        if (strstr(temp, ":") != NULL) {
            temp[strlen(temp) - 1] = '\0';
            for (i = 0; i < 33; i++) { /* 33 is number of saved words*/
                if (strcmp(savedWords[i], input->val) == 0) {
                    printf("illigal label name");
                    return false;
                }
            }
        }
    }
    return false;
}

int checkParam(int funcNum, node_t* input) {
    char* rs = input->val;
    char* rt = input->next->val;
    char* rd = input->next->next->val;
    char param1 = false, param2 = false, param3 = false;
    int i;

    switch (funcNum) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            for (i = 0; i < 32; i++) {  /* 32 is number of registerd */
                if (strcmp(rs, registerList[i]) == 0 && param1 == false) {
                    param1 = true;
                }
                if (strcmp(rt, registerList[i]) == 0 && param2 == false) {
                    param2 = true;
                }
                if (strcmp(rd, registerList[i]) == 0 && param3 == false) {
                    param3 = true;
                }
            }
    }
    if (param1 && param2 && param3) {
        return true;
    }
    return false;
}