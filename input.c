#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

node_t* getLine(FILE* fp, flags* flag) { /* saves each word a new node */
    int i = 0, j = 0;
    node_t* head = initList();
    node_t* node = head;
    char temp, comma = false, label = false, instruction = true;
    char* currVal = node->val;
    char tempLine[MAX_LINE_LENGTH + 2] = {0};
    if (head) {
        fgets(tempLine, MAX_LINE_LENGTH + 2, fp);

        if (strchr(tempLine, '\n') == NULL) {
            if (strlen(tempLine) > MAX_LINE_LENGTH) { /* Check if line exceeds allowed length or last line in file */
                printf("\nLine: %d - Line too long. Max line length is %d", flag->line, MAX_LINE_LENGTH - 1);
                freeInputList(head);
                flag->firstPass = false;
                return NULL;
            } else if (feof(fp) && tempLine[0] == '\0') { /* check if last line is empty */
                flag->lastLine = true;
                freeInputList(head);
                return NULL;
            }
        }
        temp = tempLine[0];

        if (temp == '\n') { /* if empty line */
            freeInputList(head);
            return NULL;
        }

        while (temp != '\n') {
            if (temp == '\0') {
                flag->lastLine = true;
                return head;
            }
            if (temp == ';') {
                if (i != 0) {
                    printf("\nLine: %d - Illegal character", flag->line);
                    freeInputList(head);
                    flag->firstPass = false;
                    return NULL;
                } else {
                    freeInputList(head);
                    return NULL;
                }
            }
            if (temp == ':') {
                label = true;
                instruction = false;
            }
            if (comma && temp == ',') {
                flag->firstPass = false;
                printf("\nLine: %d - Consecutive commas", flag->line);
                freeInputList(head);
                return NULL;
            }
            if (isspace(temp)) {
                while (isspace(temp) && temp != '\n') {
                    i++;
                    temp = tempLine[i];
                }
                if (!comma && (label || instruction)) {
                    if (label) {
                        label = false;
                        instruction = true;
                    } else if (instruction) {
                        instruction = false;
                    }
                    node->next = addNode();
                    if (node->next != NULL) {
                        node = node->next;
                        currVal = node->val;
                        *(currVal) = -1;
                        j = 0;
                    }
                } else if (temp != ',' && temp != '\n' && temp != EOF && temp != '\0') {
                    printf("\nLine: %d - Missing comma", flag->line);
                    freeInputList(head);
                    flag->firstPass = false;
                    return NULL;
                }
                /* else if ((isspace(prevChar) || temp == ',') && *currVal != -1) {
                    node->next = addNode();
                    if (node->next != NULL) {
                        node = node->next;
                        currVal = node->val;
                        *(currVal) = -1;
                        j = 0;
                        prevChar = temp;
                    } else {
                        printf("Memory allocation error");
                        exit(0);
                    } 
                }
                 */
                continue;
            } else if (temp == ',') {
                comma = true;
                i++;
                temp = tempLine[i];
                node->next = addNode();
                node = node->next;
                currVal = node->val;
                j = 0;
                continue;
            } else {
                comma = false;
                *(currVal + j) = temp;
                j++;
                i++;
                temp = tempLine[i];
            }
        }
        return head;
    } else {
        printf("Memory allocation error");
        exit(0);
    }
}

char isLabel(node_t* input, flags* flag, sym_t* symbol) {
    input->val[strlen(input->val) - 1] = '\0';
    if ((strlen(input->val) <= LABEL_MAX_LENGTH) &&
        (isAlphaNumeric(input->val)) &&
        (!isReserved(input->val, flag)))
        return true;
    else {
        flag->firstPass = false;
        printf("\nLine: %d - Illegal label name", flag->line);
    }
    return false;
}

char isColon(char* input, flags* flag) {
    if (strchr(input, ':') != NULL) return true;
    return false;
}
