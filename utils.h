#ifndef __UTILS_H
#define __UTILS_H

#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false     /* 45 = '0', 57 = '9' */

enum {
    false = 0,
    true = 1,
    undef_address = 666 /* 100 + 4X = 666 => X is not an integer, hence this address cannot be naturally created by the assembler. i.e IC != 666 */
};

typedef struct flag { /* this struct contains all the differnt flags and indicators for the assembly operation */
    char label;
    char params;
    char stop;
    char pass;
    char firstPass;
    char direction;
    unsigned int line;
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;

node_t *initList();
node_t* addNode();

char isAlphaNumeric();
char isReserved();
void isDeclared();
void freeList();

/* these function are in input.c */
node_t* getLine();
char isLabel();


#endif