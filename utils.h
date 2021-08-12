/*
* This file contains utility and input structs
*/

#ifndef __UTILS_H
#define __UTILS_H

#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false     /* 48 = '0', 57 = '9' */

typedef enum bool {
    false = 0,
    true = 1
} bool;

/* this struct contains all the differnt flags and indicators for the assembly operation */
typedef struct flag {
    bool label;        /* if a label was found */
    bool firstPass;    /* if the 1st pass was successful */
    bool secondPass;   /* if the 2nd pass was successful */
    bool direction;    /* if its a directive line */
    bool lastLine;     /* marks last line in input file */
    bool external;     /* this marks whether to create the .ext fie */
    bool entry;        /* this marks whether to create the .ent fie */
    bool isExt;        /* this marks whether current label being processed is external is external */
    unsigned int line; /* indicates which line is being processed */
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;

#endif