#ifndef __DIRECTIVE_H 
#define __DIRECTIVE_H

typedef enum directive{
    db = 0,
    dh = 1,
    dw = 2,
    asciz = 3,
    extern = 4,
    entry = 5
} dir;

/* directive structs*/
typedef struct byte {
    unsigned int val : 8;

} byte_t;
typedef struct half_word {
    unsigned int val : 16;
} half_word_t;

typedef struct word {
    unsigned int val : 32;
} word_t;

typedef struct data_img{
    byte_t byte;
    half_word_t hw;
    word_t word;
    struct data_img *next;
}data_t;


#endif