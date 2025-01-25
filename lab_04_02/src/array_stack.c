#include "array_stack.h"

void create_arr_stack(array_stack_t *stack)
{
    stack->topl_p = stack->data;
    stack->topr_p = stack->data + MAX_ARR_STACK_SIZE;
}

int pushl(array_stack_t *stack, double value) 
{ 
    if (stack->topl_p >= stack->topr_p) 
        return STACK_OVERFLOW;
    *((stack->topl_p)++) = value; 
    return STACK_OK;
}

int pushr(array_stack_t *stack, double value) 
{
    if (stack->topl_p >= stack->topr_p) 
        return STACK_OVERFLOW;
    *(--(stack->topr_p)) = value; 
    return STACK_OK;
}

int popl(array_stack_t *stack) 
{ 
    if (stack->topl_p <= stack->data) 
        return STACK_UNDERFLOW;
    (stack->topl_p)--;
    return STACK_OK;
}

int popr(array_stack_t *stack) 
{ 
    if (stack->topr_p >= stack->data + MAX_ARR_STACK_SIZE) 
        return STACK_UNDERFLOW;
    (stack->topr_p)++; 
    return STACK_OK;
}

int topl(array_stack_t *stack, double *value)
{
    if (stack->topl_p <= stack->data) 
        return STACK_UNDERFLOW;
    (*value)=*(stack->topl_p - 1);
    return STACK_OK;
}

int topr(array_stack_t *stack, double *value)
{
    if (stack->topr_p >= stack->data + MAX_ARR_STACK_SIZE) 
        return STACK_UNDERFLOW;
    *value=*(stack->topr_p); 
    return STACK_OK;
}
