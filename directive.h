#ifndef __DIRECTIVE_H
#define __DIRECTIVE_H

/* this enum is for easier readability of the code int the first pass */
enum {
    db = 0,    /* declare a location in the memory for 1 byte for each of the inputs, seperate by "comma". the label points the first item in the array. */
    dh = 1,    /* declare a location in the memory for 2 byte for each of the inputs, seperate by "comma". the label points the first item in the array. */
    dw = 2,    /* declare a location in the memory for 4 byte for each of the inputs, seperate by "comma". the label points the first item in the array. */
    asciz = 3, /* declare a string. the label points the first item in the array. the string is between "" , in the end \0 */
    ext = 4,   /* declare external label */
    ent = 5    /*declare local label */
};

typedef enum mem_type {
    byte = 0,
    half_word = 1,
    word = 2,
    str = 3
} mem;

/* directive structs - contains the different variable sizes available to this machine */
typedef struct DIRECTIVE {
    int byte : 8;
    int half_word : 16;
    int word : 32;
    mem flag; /* indicates which directive was entered (.asciz, .db, .dh, .dw) */
    struct DIRECTIVE *next;
} dir_t;

typedef struct DIRECTIVE *data_ptr;

#endif