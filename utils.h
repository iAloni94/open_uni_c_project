/*
* This file contains utility and input structs
*/

#ifndef __UTILS_H
#define __UTILS_H

#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false     /* 48 = '0', 57 = '9' */

#include "assembler.h"

enum {
    false = 0,
    true = 1
};

typedef struct flag { /* this struct contains all the differnt flags and indicators for the assembly operation */
    char label;
    char firstPass;
    char secondPass;
    char direction;
    char lastLine;
    char external;
    char entry;
    unsigned int line;
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;

#endif