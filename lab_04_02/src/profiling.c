#include "profiling.h"
#include "array_stack.h"
#include "list_stack.h"

int time_measurement(void)
{
    struct timespec start, end;
    int64_t time_delta_push = 0;
    int64_t time_delta_pop = 0;

    array_stack_t arr_stack;
    create_arr_stack(&arr_stack);
    list_stack_t list_stack;
    create_stack(&list_stack);

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_STACK_SIZE; i++)
        pushl(&arr_stack, 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    time_delta_push += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
    time_delta_push /= MAX_STACK_SIZE;   

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_STACK_SIZE; i++)
        popl(&arr_stack);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_pop += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_pop /= MAX_STACK_SIZE;

    printf("Стек на массиве:\n");
    printf("push: %zu(нс)\npop: %zu(нс)\n", time_delta_push, time_delta_pop);
    printf("Размер элемента %zu(байт)\n", sizeof(double));

    printf("\n");

    time_delta_push = 0;
    time_delta_pop = 0;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_STACK_SIZE; i++)
        push(&list_stack, 1);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_push += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_push /= MAX_STACK_SIZE;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    for (size_t i = 0; i < MAX_STACK_SIZE; i++)
        pop(&list_stack);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    time_delta_pop += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

    time_delta_pop /= MAX_STACK_SIZE;

    printf("Стек на списке:\n");
    printf("push: %zu(нс)\npop: %zu(нс)\n", time_delta_push, time_delta_pop);
    printf("Размер элемента %zu(байт)\n", sizeof(node_t));

    destroy_stack(&list_stack);
    
    return EXIT_SUCCESS;
}
