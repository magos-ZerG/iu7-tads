#ifndef LIST_STACK_H__
#define LIST_STACK_H__

#include "errno.h"
#include "stdlib.h"

#define MAX_STACK_SIZE 10000

typedef struct node_t
{
    double data;
    struct node_t* next;
} node_t;

typedef struct
{
    node_t *head;
    size_t size;
} list_stack_t;

void create_stack(list_stack_t *stack);

void destroy_stack(list_stack_t *stack);

int push(list_stack_t *stack, double value);

int pop(list_stack_t *stack);

int top(list_stack_t *stack, double *value);

#endif //LIST_STACK_H__