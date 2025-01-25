#ifndef LIST_QUEUE_H__
#define LIST_QUEUE_H__

#include "errno.h"
#include "config.h"
#include <stdlib.h>

typedef struct node_t
{
    double data;
    struct node_t* next;
} node_t;

typedef struct 
{
    node_t *front_ptr;
    node_t *back_ptr;
    size_t queue_size;
} list_queue_t;

void create_queue(list_queue_t *queue);
void destroy_queue(list_queue_t *queue);

int push_back(list_queue_t *queue, double value);
int pop_front(list_queue_t *queue);

int front(list_queue_t *queue, double *value);
int back(list_queue_t *queue, double *value);

#endif //LIST_QUEUE_H__
