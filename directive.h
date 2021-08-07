#ifndef __DIRECTIVE_H
#define __DIRECTIVE_H

typedef enum directive {
    db = 0,
    dh = 1,
    dw = 2,
    asciz = 3,
    ext = 4,
    ent = 5
} dir;

/* directive structs*/
typedef union DIRECTIVE {
    unsigned byte : 8;
    unsigned int half_word : 16;
    unsigned int word : 32;
} dir_t;

typedef union DIRECTIVE *data_ptr;

#endif