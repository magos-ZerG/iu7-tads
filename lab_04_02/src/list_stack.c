#include "list_stack.h"

static node_t *create_node(int value)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));
    if (node == NULL)
        return NULL;
    node->data = value;
    node->next = NULL;
    return node;
}

void create_stack(list_stack_t *stack)
{
    stack->head = NULL;
    stack->size = 0;
}

void destroy_stack(list_stack_t *stack)
{
    int rc = pop(stack);
    while (rc == STACK_OK)
        rc = pop(stack);    
}

int push(list_stack_t *stack, double value)
{
    if (stack->size >= MAX_STACK_SIZE)
        return STACK_UNDERFLOW;
    node_t *new_node = create_node(value);
    if (new_node == NULL)
        return STACK_OVERFLOW;
    
    new_node->next = stack->head;
    stack->head = new_node;
    (stack->size)++;

    return STACK_OK;
}

int pop(list_stack_t *stack)
{
    if (stack->head == NULL)
        return STACK_UNDERFLOW;
    
    node_t *tmp = stack->head;
    stack->head = stack->head->next;
    free(tmp); 
    (stack->size)--;

    return STACK_OK;
}

int top(list_stack_t *stack, double *value)
{
    if (stack->head == NULL)
        return STACK_UNDERFLOW;
    
    *value = stack->head->data;

    return STACK_OK;
}