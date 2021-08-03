#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler.h"
#include "utils.h"

node_t* getLine(FILE* fp, flags* flag) { /* saves each word a new node */
    int i = 0, j = 0;
    node_t* head = initList();
    node_t* node = head;
    char temp, prevChar, comma = false;
    char* currVal = node->val;
    char tempLine[MAX_LINE_LENGTH] = {0};
    if (head) {
        fgets(tempLine, MAX_LINE_LENGTH, fp);

        if (strchr(tempLine, '\n') == NULL) { /* Check if line exceeds allowed length or last line in file */
            if (feof(fp)) {
                flag->lastLine = true;
                freeInputList(head);
                return NULL;
            } else {
                printf("\nLine: %d - Line too long. Max line length is %d", flag->line, MAX_LINE_LENGTH - 1);
                flag->firstPass = false;
                return NULL;
            }
        }
        temp = tempLine[0];
        prevChar = temp;

        if (temp == '\n') { /* if empty line */
            free(head);
            return NULL;
        }

        while (temp != '\n') {
            if (temp == ';') {
                return NULL;
            }
            if (isspace(temp)) {
                while (isspace(temp) && temp != '\n') {
                    prevChar = temp;
                    i++;
                    temp = tempLine[i];
                }
                if (comma && temp == ',') {
                    comma = false;
                    flag->firstPass = false;
                    printf("\nLine: %d - consecutive commas", flag->line);
                } else if ((isspace(prevChar) || temp == ',') && *currVal != -1) {
                    node->next = addNode();
                    if (node->next != NULL) {
                        node = node->next;
                        currVal = node->val;
                        *(currVal) = -1;
                        j = 0;
                        prevChar = temp;
                    } else {
                        printf("Memory allocation error");
                        flag->firstPass = false;
                        return NULL;
                    }
                }
                continue;
            } else if (temp == ',') {
                comma = true;
                prevChar = temp;
                i++;
                temp = tempLine[i];
                continue;
            } else {
                comma = false;
                *(currVal + j) = temp;
                j++;
                i++;
                prevChar = temp;
                temp = tempLine[i];
            }
        }
        return head;
    } else {
        printf("Memory allocation error");
        flag->firstPass = false;
        return NULL;
    }
}

char isLabel(node_t* input, flags* flag) {
    char* temp = input->val;
    if (strstr(temp, ":") != NULL) {
        input->val[strlen(temp) - 1] = '\0';
        if (strlen(temp) <= LABEL_MAX_LENGTH && IS_LETTER(*temp) && isAlphaNumeric(temp) && !isReserved(temp, flag))
            return true;
        else {
            flag->firstPass = false;
            printf("\nLine: %d - Illigal label name", 1);
        }
    }
    return false;
}
