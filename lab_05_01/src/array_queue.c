#include "array_queue.h"

void create_arr_queue(array_queue_t *queue)
{
    queue->back_ptr = queue->data;
    queue->front_ptr = queue->data;
    queue->queue_size = 0;
}

int push_back_arr(array_queue_t *queue, double value)
{
    if (queue->queue_size >= MAX_QUEUE_SIZE)
        return QUEUE_OVERFLOW;

    if (queue->back_ptr == queue->data + MAX_QUEUE_SIZE)
    {
        memmove(queue->data, queue->front_ptr, queue->queue_size * sizeof(double));
        queue->front_ptr = queue->data;
        queue->back_ptr = queue->front_ptr + queue->queue_size;
    }

    *(queue->back_ptr) = value;
    queue->back_ptr++;
    (queue->queue_size)++;

    return QUEUE_OK;
}

int pop_front_arr(array_queue_t *queue)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;
    
    queue->front_ptr++;
    (queue->queue_size)--;

    return QUEUE_OK;
}

int front_arr(array_queue_t *queue, double *value)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;

    if (queue->front_ptr == queue->data + MAX_QUEUE_SIZE)
        return QUEUE_OVERFLOW;

    (*value) = *(queue->front_ptr);

    return QUEUE_OK;
}

int back_arr(array_queue_t *queue, double *value)
{
    if (queue->queue_size <= 0)
        return QUEUE_UNDERFLOW;

    (*value) = *(queue->back_ptr - 1);

    return QUEUE_OK;
}
