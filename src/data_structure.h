#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 80
#include "binary.h"

enum
{
    BLANK,
    COMMENT,
    DIRECTIVE,
    INSTRUCTION
};

typedef struct line *Line; /* node in doubly Linked List */

struct line
{
    int address;
    char text[MAX_LINE_LENGTH + 1];
    Binary bin;
    int type;
};

typedef struct node *Node; /* pointer to node */

struct node /* node in doubly Linked List */
{
    Line line;
    Node next;
};

typedef struct linkedList *LinkedList;

struct linkedList /* "Linked List" Data Structure */
{
    Node head; /* pointer to head of list */
    Node tail; /* pointer to end of list */
};

int print_list(LinkedList list);
int pushNode(LinkedList list, Node p);
LinkedList initList();
Node createNode(Line line);
Line createLine(char *text);

#endif