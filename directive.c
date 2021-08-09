#include <stdlib.h>

#include "global.h"

#define IN_RANGE_BYTE(n) n >= -128 && n <= 127 ? true : false
#define IN_RANGE_H_WORD(n) n >= -32768 && n <= 32767 ? true : false
#define IN_RANGE_WORD(n) n >= -2147483648 && n <= 2147483647 ? true : false /* -2,147,483,648 to 2,147,483,647	 */
#define TWO_COMP(n) (~n) + 1

dir_t *save_byte(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    while (node != NULL) {
        int temp = atoi(node->val);
        if (IN_RANGE_BYTE(temp)) {
            dataImg->byte = temp;
            dataImg->address = *DC;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
        } else {
            printf("\nLine: %d - Integer too large for 1 byte size", flag->line);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}

dir_t *save_half_word(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    while (node != NULL) {
        int temp = atoi(node->val);
        if (IN_RANGE_H_WORD(temp)) {
            dataImg->half_word = temp;
            dataImg->address = *DC;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
        } else {
            printf("\nLine: %d - Integer too large for half-word size", flag->line);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}

dir_t *save_word(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    while (node != NULL) {
        int temp = atoi(node->val);
        if (IN_RANGE_WORD(temp)) {
            dataImg->word = temp;
            dataImg->address = *DC;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
        } else {
            printf("\nLine: %d - Integer too large for half-word size", flag->line);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}