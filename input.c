#include "input.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char* val;
    struct node* next;
} node_t;

enum {
    R = 1,
    I = 2,
    J = 3
};

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

void getLine(FILE* fp) { /* saves each word a new node */
  
    node_t* head = initList();
    node_t* node = head;
    char temp = fgetc(fp);
    int i, j = 0;
    char* currVal = node->val;
    for (i = 0; i < 100; i++) {
        if (temp == '\n') {
            return;

        } else if (isspace(temp)) {
            temp = fgetc(fp);
            node->next = addNode();
            node = node->next;
            currVal = node->val;
            j = 0;
            while (isspace(temp) && temp != '\n') {
                temp = fgetc(fp);
            }
            continue;
        } else {
            *(currVal + j) = temp;
            j++;
            temp = fgetc(fp);
        }
    }
}