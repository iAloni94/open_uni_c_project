/*
* This file contains utility and input structs
*/

#ifndef __UTILS_H
#define __UTILS_H

#define CAPITAL_A_ASCII_VALUE 65
#define LOWER_CASE_Z_ASCII_VALUE 122
#define ZERO_ASCII_VALUE 48
#define NINE_ASCII_VALUE 57

#define MASK_2_BTYE 0xFFFF /* ..00 0000 1111 1111 1111 1111 */

#define IS_LETTER(c) (c >= CAPITAL_A_ASCII_VALUE && c <= LOWER_CASE_Z_ASCII_VALUE) ? true : false
#define IS_NUM(c) (c >= ZERO_ASCII_VALUE && c <= NINE_ASCII_VALUE) ? true : false

typedef enum bool {
    false = 0,
    true = 1
} bool;

/* this struct contains all the differnt flags and indicators for the assembly operation */
typedef struct flag {
    char label;        /* if a label was found */
    char firstPass;    /* if the 1st pass was successful */
    char secondPass;   /* if the 2nd pass was successful */
    char direction;    /* if its a directive line */
    char lastLine;     /* marks last line in input file */
    char external;     /* this marks whether to create the .ext fie */
    char entry;        /* this marks whether to create the .ent fie */
    char isExt;        /* this marks whether current label being processed is external is external */
    unsigned int line; /* indicates which line is being processed */
} flags;

typedef struct node { /* input nodes */
    char *val;
    struct node *next;
} node_t;

#endif