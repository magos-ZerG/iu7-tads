#ifndef ARRAY_STACK_H__
#define ARRAY_STACK_H__

#include "errno.h"
#include "stdlib.h"

#define MAX_ARR_STACK_SIZE 10000

typedef struct
{
    double *topl_p;
    double *topr_p;
    double data[MAX_ARR_STACK_SIZE];
} array_stack_t;

void create_arr_stack(array_stack_t *stack);

int pushl(array_stack_t *stack, double value);

int pushr(array_stack_t *stack, double value); 

int popl(array_stack_t *stack);

int popr(array_stack_t *stack);

int topl(array_stack_t *stack, double *value);

int topr(array_stack_t *stack, double *value);

#endif //LIST_STACK_H__