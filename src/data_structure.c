#include "data_structure.h"

Line createLine(char *text)
{
    Line line = (Line)malloc(sizeof(struct line));
    if (line)
    {
        strcpy(line->text, text);
        line->text[MAX_LINE_LENGTH] = '\0';
    }
    return line;
}

Node createNode(Line line)
{
    Node new_node = (Node)malloc(sizeof(struct node)); /* pointer to memory for the new node */
    if (new_node)                                      /* if new node created successfully update its values */
    {
        new_node->line = line;
        new_node->next = NULL; /* make next pointer to NULL */
    }
    return new_node; /* return the new ptr or NULL if there is no memory */
}

LinkedList initList() /* initialize LinkedList */
{
    LinkedList list = (LinkedList)malloc(sizeof(LinkedList)); /* pointer to memory for the Linked List */
    list->head = NULL;                                        /* head points to the first node */
    list->tail = NULL;                                        /* tail points to the last node */
    return list;                                              /* the pointer for the Linked List */
}

int pushNode(LinkedList list, Node node) /* add new node to the end of the Linked List */
{
    if (!list->head)
    {
        list->head = node;
        list->tail = node;
    }
    else
    {
        list->tail->next = node; /* add new node after tail */
        list->tail = node;       /* update the new tail of the list */
    }
    return 0;
}

int print_node(Node node) /* printing one node */
{
    if (!node) /* if node is NULL */
    {
        return 1;
    }
    printf("%s", node->line->text); /* print line */
    return 0;                       /* printed successfully */
}

int print_list(LinkedList list) /* printing the entire List */
{
    Node p = list->head;       /* start of the list */
    while (print_node(p) == 0) /* print node after node until NULL */
    {
        p = p->next; /* gets the next node */
    }
    return 0;
}