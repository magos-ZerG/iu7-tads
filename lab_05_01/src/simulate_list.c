#include "simulate_list.h"

static int gen_T1(void);
static int gen_T2(void);
static int gen_T3(void);

int simulate_list(int verbose)
{
    list_queue_t list_queue;

    int time_loss = 0;
    int handler = gen_T3();
    int req1_timeout = 0;
    int req2_num_timeout = 0;
    int req1_count = 0;
    int req2_count = 1;
    int req1_time = 0;
    int req2_time = 0;
    double tmp_t = 0;
    int rc;

    double avg_queue_len = 0;
    int max_queue_len = 0;

    struct timespec start, end;

    srand(time(NULL));

    int repeats_num;
    if (verbose)
        repeats_num = 1;
    else 
        repeats_num = 1000;

    int64_t time_delta = 0;

    for (size_t i = 0; i < repeats_num; i++)
    {
        time_loss = 0;
        handler = gen_T3();
        req1_timeout = 0;
        req2_num_timeout = 0;
        req1_count = 0;
        req2_count = 1;
        req1_time = 0;
        req2_time = 0;
        tmp_t = 0;

        avg_queue_len = 0;
        max_queue_len = 0;

        create_queue(&list_queue);

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        while (req1_count < MAX_REQ_COUNT)
        {
            while (req1_timeout <= 0)
            {
                tmp_t = gen_T2();
                rc = push_back(&list_queue, tmp_t);
                if (rc != QUEUE_OK)
                    return rc;
                req1_timeout = gen_T1();
            }
            if (req1_timeout > 0)
                req1_timeout--;

            while (handler == 0)
            {
                if (req2_num_timeout >= 4 || list_queue.queue_size == 0)
                {
                    handler = gen_T3();
                    req2_num_timeout = 0;
                    req2_count++;
                    req2_time += handler;
                }
                else if (list_queue.queue_size > 0)
                {
                    if (verbose)
                        if (req1_count % 100 == 0 && req1_count >= 100)
                        {
                            printf("Вошло заявок первого типа: %zu\n", req1_count + list_queue.queue_size);
                            printf("Вышло заявок первого типа: %d\n", req1_count);
                            printf("Обращений заявок второго типа: %d\n", req2_count);
                            printf("Текущая длина очреди: %zu\n", list_queue.queue_size);
                            printf("Средняя длина очреди: %lf\n", avg_queue_len / (req1_time + req2_time + time_loss));
                            printf("Среднее время заявки: %lf\n", (double)(req1_time + req2_time) / (req1_count + req2_count));
                            printf("\n");
                        }   
                    rc = front(&list_queue, &tmp_t);
                    if (rc != QUEUE_OK)
                        return rc;
                    rc = pop_front(&list_queue);
                    if (rc != QUEUE_OK)
                        return rc;
                    handler = tmp_t;
                    req2_num_timeout++;
                    req1_count++;
                    req1_time += handler;             
                }
                else
                    time_loss++;
            }
            if (handler > 0)
                handler--;
            avg_queue_len += list_queue.queue_size;
            if (list_queue.queue_size > max_queue_len)
                max_queue_len = list_queue.queue_size;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        time_delta += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

        destroy_queue(&list_queue);
    }

    if (verbose)
    {
        printf("Время моделирования: %d\n", req1_time + req2_time + time_loss);
        printf("Время простоя: %d\n", time_loss);
        printf("Вошло заявок первого типа: %zu\n", req1_count + list_queue.queue_size);
        printf("Вышло заявок первого типа: %d\n", req1_count);
        printf("Обращений заявок второго типа: %d\n", req2_count);
        double theor_time = (MAX_REQ_COUNT * (T2_MAX + T2_MIN) / 2.0 + MAX_REQ_COUNT / 4.0 * (T3_MAX + T3_MIN) / 2.0);
        double tolerance = (req1_time + req2_time + time_loss - theor_time) / theor_time;
        printf("Погрешность: %lf\n", fabs(tolerance) * 100);
    }
    else
    {
        printf("Время симуляции: %zu\n", time_delta / repeats_num);
        printf("Затраченная память %zu\n", max_queue_len * sizeof(double));
    }

    return EXIT_SUCCESS;
}

static int gen_T1(void)
{
    return rand() % (T1_MAX - T1_MIN + 1) + T1_MIN;
}

static int gen_T2(void)
{
    return rand() % (T2_MAX - T2_MIN + 1) + T2_MIN;
}

static int gen_T3(void)
{
    return rand() % (T3_MAX - T3_MIN + 1) + T3_MIN;
}
