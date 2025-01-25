#include "array_stack.h"
#include "list_stack.h"
#include "profiling.h"
#include <stdlib.h>
#include <stdio.h>

void print_menu(void);

int main(void)
{
    array_stack_t arr_stack;
    create_arr_stack(&arr_stack);
    list_stack_t list_stack;
    create_stack(&list_stack);

    double value;

    printf("Программа для демонстрации работы стека\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;
    node_t *freed_adresses[MAX_STACK_SIZE];
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
                rc = pushl(&arr_stack, value);
                if (rc != STACK_OK)
                    printf("Переполнение стека\n");
                else
                    printf("Элемент успешно добавлен\n");
                break;
            case 2:
                rc = popl(&arr_stack);
                if (rc != STACK_OK)
                    printf("Левый стек пуст\n");
                else
                    printf("Число удалено\n");
                break;
            case 3:
                rc = topl(&arr_stack, &value);
                if (rc != STACK_OK)
                    printf("Левый стек пуст\n");
                else
                    printf("Вершина: %lf\n", value);
                break;
            case 4:
                if (arr_stack.topl_p <= arr_stack.data) 
                    printf("Левый стек пуст\n");
                else
                {
                    printf("Левый стек: ");
                    for (double *curr_p = arr_stack.topl_p; curr_p > arr_stack.data; curr_p--)
                        printf("%lf ", *(curr_p - 1));
                    printf("\n");
                }             
                break;
            case 5:
                printf("Введите число: ");
                while (scanf("%lf", &value) != 1 || getchar() != '\n')
                {
                    printf("Невалидное число.\n");
                    while (getchar() != '\n');
                }
                rc = pushr(&arr_stack, value);
                if (rc != STACK_OK)
                    printf("Переполнение стека\n");
                else
                    printf("Элемент успешно добавлен\n");
                break;
            case 6:
                rc = popr(&arr_stack);
                if (rc != STACK_OK)
                    printf("Правый стек пуст\n");
                else
                    printf("Число удалено\n");
                break;
            case 7:
                rc = topr(&arr_stack, &value);
                if (rc != STACK_OK)
                    printf("Правый стек пуст\n");
                else
                    printf("Вершина: %lf\n", value);
                break;
            case 8:
                if (arr_stack.topr_p >= arr_stack.data + MAX_ARR_STACK_SIZE) 
                    printf("Правый стек пуст\n");
                else
                {
                    printf("Правый стек: ");
                    for (double *curr_p = arr_stack.topr_p; curr_p < arr_stack.data + MAX_ARR_STACK_SIZE; curr_p++)
                        printf("%lf ", *curr_p);
                    printf("\n");
                }   
                break;
            case 9:
                printf("Введите число: ");
                while (scanf("%lf", &value) != 1 || getchar() != '\n')
                {
                    printf("Невалидное число.\n");
                    while (getchar() != '\n');
                }
                rc = push(&list_stack, value);
                if (rc != STACK_OK)
                {
                    printf("Переполнение стека\n");
                }
                else
                {
                    printf("Элемент успешно добавлен\n");
                }
                break;
            case 10:
                if (list_stack.size > 0)
                    freed_adresses[freed_adresses_len++] = list_stack.head;
                rc = pop(&list_stack);
                if (rc != STACK_OK)
                    printf("Cтек пуст\n");
                else
                    printf("Число удалено\n");
                break;
            case 11:
                rc = top(&list_stack, &value);
                if (rc != STACK_OK)
                    printf("Cтек пуст\n");
                else
                    printf("Вершина: %lf\n", value);
                break;
            case 12:
            {
                if (list_stack.head == NULL) 
                    printf("Cтек пуст\n");
                else
                {
                    printf("Правый стек:\n");
                    for (node_t *curr_node = list_stack.head; curr_node != NULL; curr_node = curr_node->next)
                        printf("Значение: %lf Адрес: %p\n", curr_node->data, curr_node);
                    printf("\n");
                }   
                break;
            }
            case 13:
                if (freed_adresses_len != 0)
                {
                    printf("Освобождённые адреса:\n");
                    for (size_t i = 0; i < freed_adresses_len; i++)
                        printf("%p\n", freed_adresses[i]);
                }
                else
                {
                    printf("С последнего добавления элемента адресов не освобождалось\n");
                }
                break;
            case 14:
                rc = time_measurement();
                if (rc != EXIT_SUCCESS)
                    printf("Ошибка выделения памяти\n");
                break;
            case 15:
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

    destroy_stack(&list_stack);

    return EXIT_SUCCESS;
}

void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Добавить число в стек слева\n");
    printf("2 - Удалить число из стека слева\n");
    printf("3 - Вывести вершину левого стека\n");
    printf("4 - Вывести левый стек\n");
    printf("5 - Добавить число в стек справа\n");
    printf("6 - Удалить число из стека справа\n");
    printf("7 - Вывести вершину правого стека\n");
    printf("8 - Вывести правый стек\n");
    printf("9 - Добавить число в стек-список\n");
    printf("10 - Удалить число из стека-списка\n");
    printf("11 - Ввести вершину стека-списка\n");
    printf("12 - Вывести стек-список\n");
    printf("13 - Освобождённые адреса\n");
    printf("14 - Анализ эффективности\n");
    printf("15 - Выход\n");
    printf("\n");
}
