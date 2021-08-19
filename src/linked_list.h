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


/**
 * @brief initialize Linked list
 * 
 * @return LinkedList 
 */
LinkedList init_list();

/**
 * @brief initialize Node for list. 
 * 
 * @param element already malloc element
 * @return Node 
 */
Node init_node(void *element);

/**
 * @brief add element to end of the list
 * 
 * @param linked_list the list to add to.
 * @param element the element to add.
 */
void add_last(LinkedList linked_list, void *element);

/**
 * @brief free the list and the nodes inside
 * 
 * @param linked_list the list to free
 * @param node_data_free pointer to function to free the nodes.
 */
void free_list(LinkedList linked_list, void (*node_data_free)(void *data));

#endif