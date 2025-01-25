#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <ctype.h>

#define MAX_DIGITS_NUM 50
#define MAX_DIGITS_IO_NUM 40

#define MAX_MANTISSA_LEN MAX_DIGITS_NUM
#define MAX_MANTISSA_IO_LEN MAX_DIGITS_IO_NUM
#define MAX_EXPONENT 99999
#define MIN_EXPONENT -99999

#define NOT_AN_INT 1
#define EXCEEDED_MAX_DIGITS_NUM 2

#define NOT_A_FLOAT 1
#define EXCEEDED_MAX_MANTISSA_LEN 2
#define INVALID_EXPONENT 3

typedef struct
{
    int sign;
    int digits[MAX_DIGITS_NUM];
} big_int_t;

typedef struct
{
    int sign;
    int mantissa[MAX_MANTISSA_LEN];
    int exponent;
} big_float_t;

int is_big_int(char *str);
void to_big_int(char *str, big_int_t *num);
int is_big_float(char *str);
void to_big_float(char *str, big_float_t *num);
void big_int_to_big_float(big_int_t *int_num, big_float_t *float_num);

void shift_arr(int *arr, size_t arr_size, int shift);
int cmp_arr(int *a, int *b, size_t size);
void sub_arr(int *minuend, int *subtrahent, size_t arr_size);

big_float_t division(big_float_t *dividend, big_float_t *divisor);

void round_big_float(big_float_t *num, size_t round_digit);
void printf_normalized(big_float_t *num);
