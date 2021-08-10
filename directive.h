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

typedef enum mem_size{
    byte = 0,
    half_word = 1,
    word = 2,
    asci = 3
} mem;

/* directive structs*/
typedef struct DIRECTIVE {
    unsigned int byte : 8;
    unsigned int half_word : 16;
    unsigned int word : 32;
    char flag; /* indicates which size memory is in use */
    struct DIRECTIVE *next;
} dir_t;

typedef struct DIRECTIVE *data_ptr;

#endif