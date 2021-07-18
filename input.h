#ifndef __INPUT_H
#define __INPUT_H

enum {
    false = 0,
    true = 1,
    r_type = 2,
    i_type = 3,
    j_type = 4
};

typedef struct node {
    char* val;
    struct node* next;
} node_t;

node_t* getLine();
char checkIfLabel();


#endif