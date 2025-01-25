#include "big_num.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFF_LEN 50

#define INVALID_STRING_ERROR 1
#define NOT_AN_INT_ERROR 2
#define EXCEEDED_MAX_DIGITS_NUM_ERROR 3
#define NOT_A_FLOAT_ERROR 4
#define EXCEEDED_MAX_MANTISSA_LEN_ERROR 5
#define INVALID_EXPONENT_ERROR 6

int main(void)
{
    int is_running = 1;
    printf("Деление целого числа на вещественное\n");
    printf("Вводимые строки не должны содержать пробелов.\n");
    while (is_running)
    {        
        big_int_t dividend;
        char dividend_buff[MAX_BUFF_LEN];
        printf("Введите в одну строку целое число, содержащее не более 40 цифр:\n");
        printf("Число должно соотвествовать следующей нотации: ^[-+]?[0-9]+$\n");
        printf("         10        20        30        40        50\n");
        printf("---------|---------|---------|---------|---------|\n");
        if (!fgets(dividend_buff, MAX_BUFF_LEN, stdin) || !strrchr(dividend_buff, '\n'))
        {
            printf("Ввод невожможно интерпретировать как валидную строку\n");
            return INVALID_STRING_ERROR;
        }

        dividend_buff[strlen(dividend_buff) - 1] = '\0';

        int rc = is_big_int(dividend_buff);
        if (rc == NOT_AN_INT)
        {
            printf("Введённая строка не является целым числом\n");
            return NOT_AN_INT_ERROR;
        }
        else if (rc == EXCEEDED_MAX_DIGITS_NUM)
        {
            printf("Число содержит более 40 цифр\n");
            return EXCEEDED_MAX_DIGITS_NUM_ERROR;
        }

        to_big_int(dividend_buff, &dividend);

        big_float_t divisor;
        char divisor_buff[MAX_BUFF_LEN];
        printf("Введите в одну строку вещественное число, содержащее не более 40 цифр в мантиссе и экспонентой по модулю не превышающей 99999:\n");
        printf("Число должно соотвествовать следующей нотации: ^[-+]?(([0-9]+([.][0-9]+)?)|([.][0-9]+)|([0-9]+[.]))([eE][-+]?[0-9]+)?$\n");
        printf("         10        20        30        40        50\n");
        printf("---------|---------|---------|---------|---------|\n");
        if (!fgets(divisor_buff, MAX_BUFF_LEN, stdin) || !strchr(divisor_buff, '\n'))
        {
            printf("Ввод невожможно интерпретировать как валидную строку\n");
            return INVALID_STRING_ERROR;
        }

        divisor_buff[strlen(divisor_buff) - 1] = '\0';

        rc = is_big_float(divisor_buff);
        if (rc == NOT_A_FLOAT)
        {
            printf("Введённая строка не является вещественным числом\n");
            return NOT_A_FLOAT_ERROR;
        }
        else if (rc == EXCEEDED_MAX_MANTISSA_LEN)
        {
            printf("Число содержит более 40 цифр в мантиссе\n");
            return EXCEEDED_MAX_MANTISSA_LEN_ERROR;
        }
        else if (rc == INVALID_EXPONENT)
        {
            printf("Переполнение экспоненты\n");
            return INVALID_EXPONENT_ERROR;
        }

        to_big_float(divisor_buff, &divisor);

        big_float_t float_dividend;
        big_int_to_big_float(&dividend, &float_dividend);

        size_t divisor_non_zero_ind = 0;
        for (; divisor_non_zero_ind < MAX_MANTISSA_LEN && divisor.mantissa[divisor_non_zero_ind] == 0; divisor_non_zero_ind++);
        size_t dividend_non_zero_ind = 0;
        for (; dividend_non_zero_ind < MAX_MANTISSA_LEN && float_dividend.mantissa[dividend_non_zero_ind] == 0; dividend_non_zero_ind++);
        
        if (divisor_non_zero_ind == MAX_MANTISSA_LEN)
            printf("Деление на 0 неопределено.\n");
        else if (dividend_non_zero_ind == MAX_MANTISSA_LEN)
        {
            printf("Результат:\n");
            printf("         10        20        30        40        50\n");
            printf("---------|---------|---------|---------|---------|\n");
            printf("0.0e0");
            printf("\n");
        }
        else
        {
            big_float_t res;

            res = division(&float_dividend, &divisor);

            if (MIN_EXPONENT > res.exponent || res.exponent > MAX_EXPONENT)
                printf("Переполнение экспоненты\n");
            else
            {
                round_big_float(&res, MAX_MANTISSA_IO_LEN);

                printf("Результат:\n");
                printf("         10        20        30        40        50\n");
                printf("---------|---------|---------|---------|---------|\n");
                printf_normalized(&res);
                printf("\n");
            }
        }

        printf("Для продолжения работы программы нажмите Enter, иначе введите произвольную строку:\n");
        if (getchar() != '\n')
            is_running = 0;
    }

    return EXIT_SUCCESS;
}
