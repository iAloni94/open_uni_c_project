#include <stdlib.h>
#include <string.h>

#include "global.h"

#define IN_RANGE_BYTE(n) n >= -128 && n <= 127 ? true : false
#define IN_RANGE_H_WORD(n) n >= -32768 && n <= 32767 ? true : false
#define INT_MAX 2147483647

char *checkStr(node_t *node, flags *flag) { /*  this funtion checks if the string is valid i.e starts and ends with a " */
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
    char c;
    int temp, i;
    while (node != NULL) {
        while ((c = *(node->val + i)) != '\0') {
            if (!IS_NUM(c) && c != '-') {
                printf("Line: %d - Operand should be an integer", flag->line);
                flag->firstPass = false;
                return dataImg;
            }
            i++;
        }
        temp = atoi(node->val);
        if (IN_RANGE_BYTE(temp)) {
            dataImg->byte = temp;
            dataImg->flag = byte;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
        } else {
            printf("\nLine: %d - Integer \"%d\" too large for 1 byte size", flag->line, temp);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}

dir_t *save_half_word(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    char c;
    int temp, i;
    while (node != NULL) {
        while ((c = *(node->val + i)) != '\0') {
            if (!IS_NUM(c) && c != '-') {
                printf("Line: %d - Operand should be an integer", flag->line);
                flag->firstPass = false;
                return dataImg;
            }
            i++;
        }
        temp = atoi(node->val);
        if (IN_RANGE_H_WORD(temp)) {
            dataImg->half_word = temp;
            dataImg->flag = half_word;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 2;
        } else {
            printf("\nLine: %d - Integer \"%d\" too large for 2 byte size", flag->line, temp);
            flag->firstPass = false;
        }
        node = node->next;
    }
    return dataImg;
}

dir_t *save_word(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    int temp, diff, i = 0;
    char c;
    while (node != NULL) {
        while ((c = *(node->val + i)) != '\0') {
            if ( c != '-' && !IS_NUM(c)) {
                printf("Line: %d - Operand should be an integer", flag->line);
                flag->firstPass = false;
                return dataImg;
            }
            i++;
        }
        /* check for int overflow */
        temp = atoi(node->val);
        if (strchr(node->val, '-')) {
            diff = (INT_MAX + temp) + 1; /* 2147483647 - 2147483648 = -1 but is in range*/
        } else {
            diff = INT_MAX - (temp);
        }
        if (diff >= 0) {
            dataImg->word = temp;
            dataImg->flag = word;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 4;
            node = node->next;
        } else {
            printf("\nLine: %d - Integer \"%d\" too large for 4 byte size", flag->line, temp);
            flag->firstPass = false;
        }
    }
    return dataImg;
}

dir_t *save_str(node_t *node, dir_t *dataImg, unsigned int *DC, flags *flag) {
    char *str = checkStr(node, flag);
    int i = 1;
    if (str != NULL) {
        while (str - (node->val + i) != 0) {
            dataImg->byte = *(node->val + i);
            dataImg->flag = asci;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
            i++;
        }
        dataImg->byte = 0; /* Null terminator */
        dataImg->flag = asci;
        *DC += 1;
        dataImg->next = calloc(sizeof(dir_t), 1);
        dataImg = dataImg->next;
        return dataImg;
    }
    return dataImg;
}