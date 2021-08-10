#include <stdlib.h>
#include <string.h>

#include "global.h"

#define IN_RANGE_BYTE(n) n >= -128 && n <= 127 ? true : false
#define IN_RANGE_H_WORD(n) n >= -32768 && n <= 32767 ? true : false
#define IN_RANGE_WORD(n) n >= -2147483648 && n <= 2147483647 ? true : false /* -2,147,483,648 to 2,147,483,647	 */
#define TWO_COMP(n) (~n) + 1

char *checkStr(node_t *node, flags *flag) { /*  this funtion checks if the string is valid ie start and ends with " */
    char *diff;
    if (node->next != NULL) {
        printf("Line: %d - extraneous operand", flag->line);
        flag->firstPass = false;
        return false;
    } else if (*(node->val) == '\"') {                /* if first char is " */
        if ((diff = strchr((node->val + 1), '\"'))) { /* if there is another " */
            if (*(diff + 1) == '\0') {                /* if the second " is last char in string */
                return diff;
            }
        }
    }
    printf("\nLine: %d - Illegal string", flag->line);
    flag->firstPass = false;
    return NULL;
}

dir_t *save_byte(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    while (node != NULL) {
        int temp = atoi(node->val);
        if (IN_RANGE_BYTE(temp)) {
            dataImg->byte = temp;
            dataImg->address = *DC;
            dataImg->flag = byte;
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
            dataImg->flag = half_word;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 2;
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
            dataImg->flag = word;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 4;
        } else {
            printf("\nLine: %d - Integer too large for half-word size", flag->line);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}

dir_t *save_char(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    char *str = checkStr(node, flag);
    int i = 1;
    if (str != NULL) {
        while (str - (node->val + i) != 0) {
            dataImg->byte = *(node->val + i);
            dataImg->address = *DC;
            dataImg->flag = asci;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
            i++;
        }
        dataImg->byte = 0; /* Null terminator */
        dataImg->address = *DC;
        dataImg->flag = asci;
        *DC += 1;
        dataImg->next = calloc(sizeof(dir_t), 1);
        dataImg = dataImg->next;
        return dataImg;
    }
    return dataImg;
}