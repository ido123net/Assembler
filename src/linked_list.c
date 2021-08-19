#include "linked_list.h"
#include "data_structure.h"

LinkedList init_list()
{
    LinkedList linked_list = malloc(sizeof(struct linked_list));
    if (linked_list != NULL)
    {
        linked_list->head = NULL;
        linked_list->tail = NULL;
    }
    return linked_list;
}

Node init_node(void *element)
{
    Node node = malloc(sizeof(struct node));
    if (node != NULL)
    {
        node->data = element;
        node->next = NULL;
        node->previous = NULL;
    }
    return node;
}

void add_last(LinkedList linked_list, void *element)
{
    Node node;

    if (!element)
    {
        fprintf(stderr, "Error malloc element: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
    node = init_node(element);

    if (!node)
    {
        fprintf(stderr, "Error malloc node: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (linked_list->tail != NULL)
    {
        Node tmp = linked_list->tail;
        node->previous = linked_list->tail;
        linked_list->tail = node;
        tmp->next = node;
    }
    else
    {
        linked_list->head = node;
        linked_list->tail = node;
    }
}

void free_list(LinkedList linked_list, void (*node_data_free)(void *data))
{
    Node node_ptr, tmp;
    if (linked_list)
    {
        node_ptr = linked_list->head;
        while (node_ptr)
        {
            tmp = node_ptr;
            node_ptr = node_ptr->next;
            tmp->previous = NULL;
            tmp->next = NULL;
            node_data_free(tmp->data);
            free(tmp);
        }
        linked_list->head = NULL;
        linked_list->tail = NULL;
    }
}
