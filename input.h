#ifndef __INPUT_H
#define __INPUT_H

enum {
    false = 0,
    true = 1
};

typedef struct node {
    char* val;
    struct node* next;
} node_t;

typedef struct flag {
    char label;
    char params;
    char stop;
} flags;



node_t* getLine();
char checkIfLabel();


#endif