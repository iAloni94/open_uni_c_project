#ifndef __INPUT_H
#define __INPUT_H

#define MAX_LINE_LENGTH 80

typedef struct node {
    char* val;
    struct node* next;
} node_t;

node_t* getLine();
char checkIfLabel();


#endif