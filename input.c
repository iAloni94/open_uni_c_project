#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

node_t* getLine(FILE* fp, flags* flag) { /* saves each word a new node */
    int i = 0, j = 0;
    node_t* head = initList();
    node_t* node = head;
    char temp;
    bool comma = false, label = false, instruction = true, directive = true, quotation_mark = false, firstNode = true;
    char* currVal = node->val;
    char tempLine[MAX_LINE_LENGTH + 2] = {0}; /* +2 because 1 for the 81 char + 1 for null terminator */
    if (head) {
        fgets(tempLine, MAX_LINE_LENGTH + 2, fp);

        if (strchr(tempLine, '\n') == NULL) {
            if (strlen(tempLine) > MAX_LINE_LENGTH) { /* Check if line exceeds allowed length or last line in file */
                printf("\nLine: %d - Line too long. Max line length is %d", flag->line, MAX_LINE_LENGTH);
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
            if (temp == '\"') quotation_mark = true;
            if (!quotation_mark) { /* if we found quotation marks, enter all folowing text to to the same node */
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
                    if (temp == '\0' || temp == '\n') {
                        return head;
                    }
                    if (!comma) { /* if there was no comma before this space */
                        if (temp == ',') comma = true;
                        if (!firstNode) {
                            if ((label || instruction || directive || comma)) {
                                if (label) {
                                    if (comma) {
                                        printf("\nLine: %d - Extraneous character after label name", flag->line);
                                        flag->firstPass = false;
                                        freeInputList(head);
                                        return NULL;
                                    }
                                    label = false;
                                } else if (instruction || directive) {
                                    instruction = false;
                                    directive = false;
                                    if (comma) { /* if its and instrcution or directive, there shold not be a comma after*/
                                        printf("\nLine: %d - extraneous comma", flag->line);
                                        flag->firstPass = false;
                                        freeInputList(head);
                                        return NULL;
                                    }
                                }
                                node->next = addNode();
                                if (node->next != NULL) {
                                    node = node->next;
                                    currVal = node->val;
                                    j = 0;
                                    if (comma) {
                                        i++;
                                        temp = tempLine[i];
                                    }
                                }
                            } else { /* if */
                                printf("\nLine: %d - Missing comma", flag->line);
                                freeInputList(head);
                                flag->firstPass = false;
                                return NULL;
                            }
                        }
                    }
                    continue;
                } else if (temp == ',') {
                    if (label) {
                        printf("\nLine: %d - Extraneous character after label name", flag->line);
                        flag->firstPass = false;
                        freeInputList(head);
                        return NULL;
                    }
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
                    firstNode = false;
                    *(currVal + j) = temp;
                    j++;
                    i++;
                    temp = tempLine[i];
                }
            } else {
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

node_t* initList() {
    node_t* head = malloc(sizeof(node_t));
    head->val = calloc(1, 100);
    head->next = NULL;
    return head;
}

node_t* addNode() {
    node_t* newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, 100);
    newNode->next = NULL;
    return newNode;
}

void freeInputList(node_t* node) {
    node_t* currNode;
    while (node != NULL) {
        currNode = node;
        node = node->next;
        free(currNode->val);
        free(currNode);
    }
}