#ifndef __UTILS_H
#define __UTILS_H


#define IS_LETTER(c) (c >= 65 && c <= 122) ? true : false /* 65 = 'A', 122 = 'z' */
#define IS_NUM(c) (c >= 48 && c <= 57) ? true : false /* 45 = '0', 57 = '9' */

char isAlphaNumeric();
char isReserved();
void isDeclared();
void freeList();

#endif