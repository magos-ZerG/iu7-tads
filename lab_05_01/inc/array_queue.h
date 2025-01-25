#ifndef ARRAY_QUEUE_H__
#define ARRAY_QUEUE_H__

#include "errno.h"
#include "config.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
    double *front_ptr;
    double *back_ptr;
    size_t queue_size;
    double data[MAX_QUEUE_SIZE];
} array_queue_t;

void create_arr_queue(array_queue_t *queue);

int push_back_arr(array_queue_t *queue, double value);
int pop_front_arr(array_queue_t *queue);

int front_arr(array_queue_t *queue, double *value);
int back_arr(array_queue_t *queue, double *value);

#endif //ARRAY_QUEUE_H__
