#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

node_t* getLine(FILE* fp, flags* flag) { /* saves each word a new node */
    int i = 0, j = 0;                    /* i follows the file input line, j follows the node value */
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
            if (!quotation_mark) {
                if (temp == ';') {
                    if (i != 0) { /* if ";" is not at the start of the line, it should not be used at all */
                        printf("\nLine: %d - Illegal character", flag->line);
                        freeInputList(head);
                        flag->firstPass = false;
                        return NULL;
                    } else { /* skip line */
                        freeInputList(head);
                        return NULL;
                    }
                }

                /* this only mark the parser to take into account that a label is being parced */
                if (temp == ':') {
                    label = true;
                    flag->label = true;
                }

                if (comma && temp == ',') { /* if the last non white char was a comma and we found another one, report error */
                    flag->firstPass = false;
                    printf("\nLine: %d - Consecutive commas", flag->line);
                    flag->label = true;
                    freeInputList(head);
                    return NULL;
                }
                if (isspace(temp)) {
                    while (isspace(temp) && temp != '\n') { /* find next non white char */
                        i++;
                        temp = tempLine[i];
                    }
                    if (temp == '\0' || temp == '\n') { /* line ended with white char */
                        return head;
                    }
                    /* if there was no comma before this space. 
                    * there are 5 possibilities, either is a label, an instruction or a directive.
                    * or a label->instruction, label->directive
                    */
                    if (!comma) {
                        if (temp == ',') comma = true; /* current char is ',' */
                        if (!firstNode) {              /* in case a label was parsed already */
                            if ((label || instruction || directive || comma)) {
                                if (label) {
                                    if (comma) { /* if there is a comma after label (with space) "LABEL:    ," */
                                        printf("\nLine: %d - Extraneous character after label name", flag->line);
                                        flag->firstPass = false;
                                        flag->label = false;
                                        freeInputList(head);
                                        return NULL;
                                    }
                                    label = false;
                                } else if (instruction || directive) {
                                    instruction = false;
                                    directive = false;
                                    if (comma) { /* if its and instrcution or directive, there shold not be a comma after */
                                        printf("\nLine: %d - extraneous comma", flag->line);
                                        flag->firstPass = false;
                                        flag->label = false;
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
                            } else {
                                printf("\nLine: %d - Missing comma", flag->line);
                                freeInputList(head);
                                flag->label = false;
                                flag->firstPass = false;
                                return NULL;
                            }
                        }
                    }
                    continue;
                } else if (temp == ',') {
                    if (label) { /* if there is a comma after label (no space) "LABEL:," */
                        printf("\nLine: %d - Extraneous character after label name", flag->line);
                        flag->firstPass = false;
                        flag->label = false;
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
            } else { /*  quotation mark found, enter all following text to to the same node */
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
    head->val = calloc(1, MAX_LINE_LENGTH);
    head->next = NULL;
    return head;
}

node_t* addNode() {
    node_t* newNode = malloc(sizeof(node_t));
    newNode->val = calloc(1, MAX_LINE_LENGTH);
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