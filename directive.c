#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

#define ASCII_QUOTATION_MARK 34

#define IN_RANGE_BYTE(n) n >= CHAR_MIN &&n <= CHAR_MAX ? true : false   /* check if n is in 8 bits range */
#define IN_RANGE_H_WORD(n) n >= SHRT_MIN &&n <= SHRT_MAX ? true : false /* chec if n is in 16 bit range */

/* This function chcek whether the directive numeral oprands is valid */
bool checkNum(node_t *input, flags *flag) {
    char c;
    int i = 0;
    if ((c = *(input->val)) == '\0') {
        printf("\nLine: %d - extraneous comma", flag->line);
        flag->firstPass = false;
        return false;
    }
    while (c != '\0') { /* check if operand is a number */
        if (c != '-' && c != '+' && !IS_NUM(c)) {
            printf("\nLine: %d - Operand should be an integer", flag->line);
            flag->firstPass = false;
            return false;
        }
        i++;
        c = *(input->val + i);
    }
    return true;
}

/* This function chcek whether the directive string oprands is valid
*  It check if the string is legal and return a pointer to the closing quotation mark.
*/
char *checkStr(node_t *input, flags *flag) { /*  this funtion checks if the string is valid i.e starts and ends with a " */
    char *second_quotation_mark, c;
    int i = 1;
    if (input->next != NULL) {
        printf("\nLine: %d - Extraneous operand", flag->line);
        flag->firstPass = false;
        return NULL;
    } else if (*(input->val) == ASCII_QUOTATION_MARK) {                 /* if first char is " */
        if ((second_quotation_mark = strchr((input->val + i), '\"'))) { /* if there is another " */
            if (*(second_quotation_mark + i) == '\0') {                 /* if the second " is last char in string */
                i++;
                c = *(input->val + i);
                while (c != ASCII_QUOTATION_MARK) { /* check if string is valid */
                    if (isprint(c)) {
                        i++;
                        c = *(input->val + i);
                    } else {
                        printf("\nLine: %d - String contains illegal characters", flag->line);
                        flag->firstPass = false;
                        return NULL;
                    }
                }
                return second_quotation_mark; /* If string is valid, return pointer to the last quotation mark */
            } else {                          /* if first char is not " */
                printf("\nLine: %d - Extraneous text after closing quotation mark", flag->line);
                flag->firstPass = false;
                return NULL;
            }
        } else { /* there was not another " */
            printf("\nLine: %d - Closing quotation mark was not found", flag->line);
            flag->firstPass = false;
            return NULL;
        }
    } else { /* if the second " wasnt the last char in string */
        printf("\nLine: %d - String should begin with a quotation mark", flag->line);
        flag->firstPass = false;
        return NULL;
    }
    printf("\nLine: %d - Illegal string", flag->line);
    flag->firstPass = false;
    return NULL;
}

/* save byte size numeral data */
dir_t *save_byte(node_t *input, dir_t *dataImg, unsigned int *DC, flags *flag) {
    int temp;
    if (input != NULL) {
        while (input != NULL) {
            if ((checkNum(input, flag))) {
                temp = atoi(input->val);
                if (IN_RANGE_BYTE(temp)) { /* check if operand in range */
                    dataImg->byte = temp;
                    dataImg->flag = byte;
                    dataImg->next = calloc(sizeof(dir_t), 1);
                    dataImg = dataImg->next;
                    *DC += 1;
                } else {
                    printf("\nLine: %d - Integer \"%d\" is outside 1 byte capacity range (-128 to 127)", flag->line, temp);
                    flag->firstPass = false;
                    return dataImg;
                }
                input = input->next;
            } else {
                return dataImg;
            }
        }
    } else {
        printf("\nLine: %d - Missig operand", flag->line);
        flag->firstPass = false;
    }
    return dataImg;
}

/* save 2 byte size numeral data */
dir_t *save_half_word(node_t *input, dir_t *dataImg, unsigned int *DC, flags *flag) {
    int temp;
    if (input != NULL) {
        while (input != NULL) {
            if ((checkNum(input, flag))) {
                temp = atoi(input->val);
                if (IN_RANGE_H_WORD(temp)) { /* check if operand in range */
                    dataImg->half_word = temp;
                    dataImg->flag = half_word;
                    dataImg->next = calloc(sizeof(dir_t), 1);
                    dataImg = dataImg->next;
                    *DC += 2;
                } else {
                    printf("\nLine: %d - Integer \"%d\" is outside 2 byte capacity range (-32768 to 32767)", flag->line, temp);
                    flag->firstPass = false;
                }
                input = input->next;
            } else {
                return dataImg;
            }
        }
    } else {
        printf("\nLine: %d - Missig operand", flag->line);
        flag->firstPass = false;
    }
    return dataImg;
}

/* save 4 byte size numeral data */
dir_t *save_word(node_t *input, dir_t *dataImg, unsigned int *DC, flags *flag) {
    int temp, diff;
    if (input != NULL) {
        while (input != NULL) {
            if ((checkNum(input, flag))) {
                /* 
            * Check for int overflow
            * Because of int overflow, if an integers value exceeds 32 bit capacity, it "wraps around" in the opposite direction
            * for example:
            * let INT_MAX = 2147483647, INT_MIN = -2147483648.
            * INT_MAX + 1 = INT_MIN
            * INT_MIN - 1 = INT_MAX
            */
                temp = atoi(input->val);
                if (strchr(input->val, '-')) {   /* if input is a negative number that exceed 32 bits, temp would be positive because of overflow */
                    diff = (INT_MAX + temp) + 1; /* INT_MAX - INT_MIN = -1 but both in range so we add 1 to result */
                } else {                         /* if input is a positive number that exceed 32 bits, temp would be negative because of overflow */
                    diff = INT_MAX - (temp);
                }
                if (diff >= 0) {
                    dataImg->word = temp;
                    dataImg->flag = word;
                    dataImg->next = calloc(sizeof(dir_t), 1);
                    dataImg = dataImg->next;
                    *DC += 4;
                } else {
                    printf("\nLine: %d - Integer \"%s\" is outside 4 byte capacity range (-2,147,483,648 to 2,147,483,647 )", flag->line, input->val);
                    flag->firstPass = false;
                }
                input = input->next;
            } else {
                return dataImg;
            }
        }
    } else {
        printf("\nLine: %d - Missig operand", flag->line);
        flag->firstPass = false;
    }
    return dataImg;
}

/* save string data */
dir_t *save_str(node_t *input, dir_t *dataImg, unsigned int *DC, flags *flag) {
    char *second_quotation_mark = checkStr(input, flag);
    int i = 1;
    if (second_quotation_mark != NULL) {
        while (second_quotation_mark - (input->val + i) != 0) { /* pointer arithmatics, subtract pointers until they are equal */
            dataImg->byte = *(input->val + i);
            dataImg->flag = str;
            dataImg->next = calloc(sizeof(dir_t), 1);
            dataImg = dataImg->next;
            *DC += 1;
            i++;
        }
        dataImg->byte = 0; /* Add null terminator */
        dataImg->flag = str;
        *DC += 1;
        dataImg->next = calloc(sizeof(dir_t), 1);
        dataImg = dataImg->next;
        return dataImg;
    }
    return dataImg;
}

void ext_handler(sym_t *symbol, node_t *input, flags *flag, unsigned int IC, unsigned int DC) {
    node_t *tempNode = calloc(1, sizeof(node_t));
    char temp[LABEL_MAX_LENGTH] = {0};

    strcpy(temp, input->val); /* getting the label ready for parsing (adding ':') */
    strcat(temp, ":");
    tempNode->val = temp;

    if (isLabel(tempNode, flag, symbol)) {
        flag->isExt = true;
        if (!isDeclared(input->val, symbol, flag)) {
            flag->external = true;
            insertLabel(symbol, input, flag, 0, 0);
        }
        flag->isExt = false;
    }
    free(tempNode);
}

/* entry */
void ent_handler(sym_t *symbol, node_t *input, flags *flag) {
    flag->entry = true;
    input = input->next;
    if (isDeclared(input->val, symbol, flag)) {
        while (symbol != NULL) {
            if (!strcmp(input->val, symbol->name)) {
                char *temp = malloc(LABEL_MAX_LENGTH);
                memcpy(temp, symbol->attribute, strlen(symbol->attribute));
                strcat(temp, ", entry");
                free(symbol->attribute);
                symbol->attribute = temp;
                break;
            }
            symbol = symbol->next;
        }
    }
}
