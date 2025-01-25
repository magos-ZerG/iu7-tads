#include "array_queue.h"
#include "list_queue.h"
#include "profiling.h"
#include "simulate_array.h"
#include "simulate_list.h"
#include <stdlib.h>
#include <stdio.h>

void print_menu(void);

int main(void)
{
    array_queue_t arr_queue;
    create_arr_queue(&arr_queue);
    list_queue_t list_queue;
    create_queue(&list_queue);

    double value;

    printf("Программа для демонстрации работы очереди\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;
    node_t *freed_adresses[MAX_QUEUE_SIZE];
    size_t freed_adresses_len = 0;

    while (is_running)
    {
        print_menu();
        printf("Выберите опцию: ");
        rc = (scanf("%d", &option) == 1 && getchar() == '\n');

        if (rc == 1)
        {
            switch (option)
            {
            case 1:
                printf("Введите число: ");
                while (scanf("%lf", &value) != 1 || getchar() != '\n')
                {
                    printf("Невалидное число.\n");
                    while (getchar() != '\n');
                }
                rc = push_back_arr(&arr_queue, value);
                if (rc != QUEUE_OK)
                    printf("Переполнение очереди\n");
                else
                    printf("Элемент успешно добавлен\n");
                break;
            case 2:
                rc = pop_front_arr(&arr_queue);
                if (rc != QUEUE_OK)
                    printf("Очередь пуста\n");
                else
                    printf("Число удалено\n");
                break;
            case 3:
                if (arr_queue.queue_size <= 0) 
                    printf("Очередь пуста\n");
                else
                {
                    printf("Очередь: ");
                    for (double *curr_p = arr_queue.back_ptr; curr_p > arr_queue.front_ptr; curr_p--)
                        printf("%lf ", *(curr_p - 1));
                    printf("\n");
                }             
                break;
            case 4:
                printf("Введите число: ");
                while (scanf("%lf", &value) != 1 || getchar() != '\n')
                {
                    printf("Невалидное число.\n");
                    while (getchar() != '\n');
                }
                rc = push_back(&list_queue, value);
                if (rc != QUEUE_OK)
                {
                    printf("Переполнение очереди\n");
                }
                else
                {
                    printf("Элемент успешно добавлен\n");
                }
                break;
            case 5:
                if (list_queue.queue_size > 0)
                    freed_adresses[freed_adresses_len++] = list_queue.front_ptr;
                rc = pop_front(&list_queue);
                if (rc != QUEUE_OK)
                    printf("Очередь пуста\n");
                else
                    printf("Число удалено\n");
                break;
            case 6:
            {
                if (list_queue.queue_size == 0) 
                    printf("Очередь пуста\n");
                else
                {
                    printf("Очередь:\n");
                    for (node_t *curr_node = list_queue.front_ptr; curr_node != NULL; curr_node = curr_node->next)
                        printf("Значение: %lf Адрес: %p\n", curr_node->data, curr_node);
                }   
                break;
            }
            case 7:
                if (freed_adresses_len != 0)
                {
                    printf("Освобождённые адреса:\n");
                    for (size_t i = 0; i < freed_adresses_len; i++)
                        printf("%p\n", freed_adresses[i]);
                }
                else
                {
                    printf("Адреса не освобождались\n");
                }
                break;
            case 8:
                rc = time_measurement();
                if (rc != EXIT_SUCCESS)
                    printf("Ошибка выделения памяти\n");
                break;
            case 9:
                rc = simulate_arr(1);
                if (rc != EXIT_SUCCESS)
                    printf("Переполнение очереди\n");
                break;
            case 10:
                rc = simulate_list(1);
                if (rc != EXIT_SUCCESS)
                    printf("Переполнение очереди\n");
                break;
            case 11:
                rc = simulate_arr(0);
                if (rc != EXIT_SUCCESS)
                    printf("Переполнение очереди\n");
                break;
            case 12:
                rc = simulate_list(0);
                if (rc != EXIT_SUCCESS)
                    printf("Переполнение очереди\n");
                break;
            case 13:
                is_running = 0;
                break;
            default:
                printf("Введена невалидная опция\n");
                while (getchar() != '\n'); 
                break;
            }
        }
        else
        {
            printf("Введена невалидная опция\n");
            while (getchar() != '\n'); 
        }
        printf("\n");
    }

    destroy_queue(&list_queue);

    return EXIT_SUCCESS;
}

void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Добавить число в очередь на массиве\n");
    printf("2 - Удалить число из очереди на массиве\n");
    printf("3 - Вывести очередь на массиве\n");
    printf("4 - Добавить число в очередь на списке\n");
    printf("5 - Удалить число из очереди на списке\n");
    printf("6 - Вывести очередь на списке\n");
    printf("7 - Освобождённые адреса\n");
    printf("8 - Анализ эффективности\n");
    printf("9 - Симуляция на массиве\n");
    printf("10 - Симуляция на списке\n");
    printf("11 - Симуляция на массиве(время)\n");
    printf("12 - Симуляция на списке(время)\n");
    printf("13 - Выход\n");
    printf("\n");
}
