#include "profiling.h"

int time_measurement(void)
{
    struct timespec start, end;
    int64_t time_delta_push = 0;
    int64_t time_delta_pop = 0;

    array_queue_t arr_queue;
    create_arr_queue(&arr_queue);
    list_queue_t list_queue;
    create_queue(&list_queue);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_QUEUE_SIZE; i++)
        push_back_arr(&arr_queue, 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    time_delta_push += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    time_delta_push /= MAX_QUEUE_SIZE;   

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_QUEUE_SIZE; i++)
        pop_front_arr(&arr_queue);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_pop += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_pop /= MAX_QUEUE_SIZE;

    printf("Очередь на массиве:\n");
    printf("push_back: %zu(нс)\npop_front: %zu(нс)\n", time_delta_push, time_delta_pop);
    printf("Размер элемента %zu(байт)\n", sizeof(double));

    printf("\n");

    time_delta_push = 0;
    time_delta_pop = 0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_QUEUE_SIZE; i++)
        push_back(&list_queue, 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_push += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_push /= MAX_QUEUE_SIZE;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_QUEUE_SIZE; i++)
        pop_front(&list_queue);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_pop += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_pop /= MAX_QUEUE_SIZE;

    printf("Очередь на списке:\n");
    printf("push_back: %zu(нс)\npop_front: %zu(нс)\n", time_delta_push, time_delta_pop);
    printf("Размер элемента %zu(байт)\n", sizeof(node_t));

    destroy_queue(&list_queue);
    
    return EXIT_SUCCESS;
}
