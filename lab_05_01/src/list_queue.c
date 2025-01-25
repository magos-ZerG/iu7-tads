#include "list_queue.h"

static node_t *create_node(double value)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
        return NULL;
    node->data = value;
    node->next = NULL;
    return node;
}

void create_queue(list_queue_t *queue)
{
    queue->back_ptr = NULL;
    queue->front_ptr = NULL;
    queue->queue_size = 0;
}

void destroy_queue(list_queue_t *queue)
{
    int rc = pop_front(queue);
    while (rc == QUEUE_OK)
        rc = pop_front(queue);    
}

int push_back(list_queue_t *queue, double value)
{
    node_t *new_node = create_node(value);
    if (new_node == NULL)
        return QUEUE_OUT_OF_MEMORY;

    if (queue->queue_size == 0) 
    {
        queue->front_ptr = queue->back_ptr = new_node;
        (queue->queue_size)++;
        return QUEUE_OK;
    }

    queue->back_ptr->next = new_node;
    queue->back_ptr = new_node;
    (queue->queue_size)++;

    return QUEUE_OK;
}

int pop_front(list_queue_t *queue)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;
    
    node_t *tmp = queue->front_ptr;

    queue->front_ptr = queue->front_ptr->next;
    if (queue->front_ptr == NULL)
        queue->back_ptr = NULL;
    free(tmp);
    (queue->queue_size)--;

    return QUEUE_OK;
}

int front(list_queue_t *queue, double *value)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;
    (*value) = queue->front_ptr->data;
    return QUEUE_OK;
}

int back(list_queue_t *queue, double *value)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;
    (*value) = queue->back_ptr->data;
    return QUEUE_OK;
}
