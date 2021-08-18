#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
    struct node *next;
    struct node *previous;
    void *data;
} * Node;

typedef struct linked_list
{
    Node head;
    Node tail;
} * LinkedList;

LinkedList init_list();

Node init_node(void *element);

void add_last(LinkedList linked_list, void *element);

void free_list(LinkedList linked_list);

#endif