#ifndef __LABEL_H
#define __LABEL_H

typedef struct symbol {
    char *name;
    char *attribute; /* code, data, external, entry */
    unsigned int address;
    struct symbol *next;
} sym_t;

typedef struct external{
    char *name;
    unsigned int address;
    struct external *next;
} ext_t;

#endif