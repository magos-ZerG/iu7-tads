#include "students.h"
#include "student.h"
#include <stdlib.h>
#include <stdio.h>

void print_menu(void);

int main(void)
{
    student_t students[MAX_STUDENTS_NUM];
    size_t students_num = 0;

    printf("Программа для обработки таблицы студентов\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;

    while (is_running)
    {
        print_menu();
        printf("Выберите опцию: ");
        rc = scanf("%d", &option);
        getchar();

        if (rc == 1)
        {
            switch (option)
            {
            case 1:
                load_data(students, &students_num);
                break;
            case 2:
                print_students_table(students, &students_num);
                break;
            case 3:
                print_sorted_key_table(students, &students_num);
                break;
            case 4:
                add_student(students, &students_num);
                break;
            case 5:
                del_students_by_surname(students, &students_num);
                break;
            case 6:
                bubble_sort_students(students, &students_num);
                break;
            case 7:
                qsort_students(students, &students_num);
                break;
            case 8:
                bubble_sort_students_by_key(students, &students_num);
                break;
            case 9:
                qsort_students_by_key(students, &students_num);
                break;
            case 10:
                time_measurement();
                break;
            case 11:
                my_filter(students, &students_num);
                break;
            case 12:
                is_running = 0;
                break;
            default:
                printf("Введена невалидная опция\n");
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

    return EXIT_SUCCESS;
}

void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Загрузить данные из файла\n");
    printf("2 - Вывести таблицу\n");
    printf("3 - Вывести отсортированную по году поступления таблицу ключей\n");
    printf("4 - Добавить запись в конец\n");
    printf("5 - Удалить записи по префиксу фамилии\n");
    printf("6 - Сортировка таблицы пузырьком по году поступления\n");
    printf("7 - Быстрая сортировка таблицы по году поступленияу\n");
    printf("8 - Сортировка с таблицей ключей пузырьком по году поступления\n");
    printf("9 - Быстрая сортировка с таблицей ключей по году поступления\n");
    printf("10 - Анализ времени\n");
    printf("11 - Вывести список студентов указанного года поступления, живущих в съёмном жилье, стоимостью меньше указанного\n");
    printf("12 - Выход\n");
    printf("\n");
}
