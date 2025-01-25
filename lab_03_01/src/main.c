#include "matrix.h"
#include "csr_matrix.h"
#include "profiling.h"
#include <stdio.h>
#include <stdlib.h>

void print_menu(void);

static void int_sum(const void *res, const void *num1, const void *num2);
static int fscan_int(FILE * const file, void * const num);
static int fprint_int(FILE * const file, void * const num);
static int fprint_csr_int(FILE * const file, void * const num);

int main(void)
{
    matrix_t matrix1 = NULL;
    matrix_t matrix2 = NULL;
    matrix_t res = NULL;
    csr_matrix_t csr_matrix1 = NULL;
    csr_matrix_t csr_matrix2 = NULL;
    csr_matrix_t csr_res = NULL;

    printf("Программа для обработки таблицы студентов\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;

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
                rc = matrix_user_fscan(&matrix1, sizeof(int), fscan_int);
                if (rc != MATRIX_OK)
                    printf("Ошибка выделения памяти\n");
                else
                    printf("Матрица №1 успешно введена\n");
                break;
            case 2:
                if (matrix1 == NULL)
                    printf("Матрица №1 не введена\n");
                else
                {   
                    printf("Матрица №1:\n");
                    matrix_fprint(stdout, matrix1, fprint_int);
                }
                break;
            case 3:
                rc = matrix_user_fscan(&matrix2, sizeof(int), fscan_int);
                if (rc != MATRIX_OK)
                    printf("Ошибка выделения памяти\n");
                else
                    printf("Матрица №2 успешно введена\n");
                break;
            case 4:
                if (matrix2 == NULL)
                    printf("Матрица №2 не введена\n");
                else
                {   
                    printf("Матрица №2:\n");
                    matrix_fprint(stdout, matrix2, fprint_int);
                }
                break;
            case 5:
                rc = csr_matrix_user_fscan(&csr_matrix1, sizeof(int), fscan_int);
                if (rc != CSR_MATRIX_OK)
                    printf("Ошибка выделения памяти\n");
                else
                    printf("CSR матрица №1 успешно введена\n");
                break;
            case 6:
                if (csr_matrix1 == NULL)
                    printf("CSR матрица №1 не введена\n");
                else
                {   
                    printf("CSR матрица №1:\n");
                    csr_matrix_fprint(stdout, csr_matrix1, fprint_csr_int);
                }
                break;
            case 7:
                rc = csr_matrix_user_fscan(&csr_matrix2, sizeof(int), fscan_int);
                if (rc != CSR_MATRIX_OK)
                    printf("Ошибка выделения памяти\n");
                else
                    printf("CSR матрица №2 успешно введена\n");
                break;
            case 8:
                if (csr_matrix2 == NULL)
                    printf("CSR матрица №2 не введена\n");
                else
                {   
                    printf("CSR матрица №2:\n");
                    csr_matrix_fprint(stdout, csr_matrix2, fprint_csr_int);
                }
                break;
            case 9:
                if (matrix1 == NULL)
                {
                    printf("Матрица №1 не введена\n");
                }
                else if (matrix2 == NULL)
                {
                    printf("Матрица №2 не введена\n");
                }
                else if (matrix1->rows != matrix2->rows || matrix1->columns != matrix2->columns)
                {
                    printf("Сложение невозможно, матрицы разного размера\n");
                }
                else
                {
                    res = matrix_sum(&matrix1, &matrix2, int_sum);
                    printf("Сумма матриц:\n");
                    matrix_fprint(stdout, res, fprint_csr_int);
                    matrix_destroy(&res);
                }
                break;
            case 10:
                if (csr_matrix1 == NULL)
                {
                    printf("CSR матрица №1 не введена\n");
                }
                else if (csr_matrix2 == NULL)
                {
                    printf("CSR матрица №2 не введена\n");
                }
                else if (csr_matrix1->rows_ptrs->size != csr_matrix2->rows_ptrs->size|| csr_matrix1->columns != csr_matrix2->columns)
                {
                    printf("Сложение невозможно, матрицы разного размера\n");
                }
                else
                {
                    csr_res = csr_matrix_sum(&csr_matrix1, &csr_matrix2, int_sum);
                    printf("Сумма CSR матриц:\n");
                    csr_matrix_fprint(stdout, csr_res, fprint_csr_int);
                    csr_matrix_destroy(&csr_res);
                }
                break;
            case 11:
                rc = time_measurement();
                if (rc != EXIT_SUCCESS)
                    printf("Ошибка выделения памяти\n");
                break;
            case 12:
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

    if(matrix1 != NULL) matrix_destroy(&matrix1);
    if(matrix2 != NULL) matrix_destroy(&matrix2);
    if(res != NULL) matrix_destroy(&res);
    if(csr_matrix1 != NULL) csr_matrix_destroy(&csr_matrix1);
    if(csr_matrix2 != NULL) csr_matrix_destroy(&csr_matrix2);
    if(csr_res != NULL) csr_matrix_destroy(&csr_res);

    return EXIT_SUCCESS;
}

void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Ввести (обновить) матрицу №1\n");
    printf("2 - Вывести матрицу №1\n");
    printf("3 - Ввести (обновить) матрицу №2\n");
    printf("4 - Вывести матрицу №2\n");
    printf("5 - Ввести (обновить) CSR матрицу №1\n");
    printf("6 - Вывести CSR матрицу №1\n");
    printf("7 - Ввести (обновить) CSR матрицу №2\n");
    printf("8 - Вывести CSR матрицу №2\n");
    printf("9 - Сложить матрицы\n");
    printf("10 - Сложить CSR матрицы\n");
    printf("11 - Анализ эффективности\n");
    printf("12 - Выход\n");
    printf("\n");
}

static void int_sum(const void *res, const void *num1, const void *num2)
{
    *(int *)res = *((int *)num1) + *((int *)num2);
}

static int fscan_int(FILE * const file, void * const num)
{
    if (fscanf(file, "%d", (int *)num) != 1)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

static int fprint_int(FILE * const file, void * const num)
{
    fprintf(file, "%10d", *((int *)num));
    return EXIT_SUCCESS;
}

static int fprint_csr_int(FILE * const file, void * const num)
{
    fprintf(file, "%d", *((int *)num));
    return EXIT_SUCCESS;
}
