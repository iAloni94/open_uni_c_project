#ifndef __UTILS_H
#define __UTILS_H

#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false     /* 45 = '0', 57 = '9' */

#define CHECK_BIT(reg, n) ((reg)&1 << n) ? true : false         /* checks if bit in nth place from right is true or false */
#define CHANGE_BIT(reg, v, n) (reg) ^= (-v ^ reg) & (1UL << n); /* chnage nth bit in reg to v (0 or 1) */

typedef struct flag { /* this struct contains all the differnt flags and indicators for the assembly operation */
    char label;
    char params;
    char stop;
    char pass;
    char firstPass;
    char direction;
    char *fileName;
    unsigned int line;
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;


char isAlphaNumeric();
char isReserved();
void isDeclared();
void freeList();

#endif