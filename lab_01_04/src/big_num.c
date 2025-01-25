#include "big_num.h"

int is_big_int(char *str)
{
    char *regex_str = "^[-+]?[0-9]+$";
    regex_t regex;
    if (regcomp(&regex, regex_str, REG_EXTENDED|REG_NOSUB) != 0)
        return 2;
    int rc = regexec(&regex, str, (size_t)0, NULL, 0);
    regfree(&regex);
    if (rc != 0)
        return NOT_AN_INT;
    
    size_t digits_num = 0;
    char *curr_smb = str;
    while (*curr_smb)
    {
        if (isdigit(*curr_smb))
            digits_num++;
        curr_smb++;
    }

    if (digits_num > MAX_DIGITS_IO_NUM)
        return EXCEEDED_MAX_DIGITS_NUM;

    return EXIT_SUCCESS;
}

void to_big_int(char *str, big_int_t *num)
{
    memset(num, 0, sizeof(*num));

    if (str[0] == '-')
        num->sign = -1;
    else
        num->sign = 1;
    
    char *curr_p = strchr(str, '\0') - 1;
    size_t ind = 0;
    while (isdigit(*curr_p))
    {
        (num->digits)[ind++] = *curr_p - '0';
        curr_p--;
    }
}

int is_big_float(char *str)
{
    char *regex_str = "^[-+]?(([0-9]+([.][0-9]+)?)|([.][0-9]+)|([0-9]+[.]))([eE][-+]?[0-9]+)?$";
    regex_t regex;
    if (regcomp(&regex, regex_str, REG_EXTENDED|REG_NOSUB) != 0)
        return 2;
    int rc = regexec(&regex, str, (size_t)0, NULL, 0);
    regfree(&regex);
    if (rc != 0)
        return NOT_A_FLOAT;

    int exponent;
    char *exp_pos = strpbrk(str, "eE");
    if (exp_pos != NULL)
    {
        exponent = atoi(exp_pos + 1);
        if (MIN_EXPONENT > exponent || exponent > MAX_EXPONENT)
            return INVALID_EXPONENT;
    }

    char *curr_smb = str;
    int digits_count = 0;
    while (*curr_smb && *curr_smb != 'e' && *curr_smb != 'E')
    {
        if (isdigit(*curr_smb))
            digits_count++;
        curr_smb++;
    }

    if (digits_count > MAX_MANTISSA_IO_LEN)
        return EXCEEDED_MAX_MANTISSA_LEN;

    return EXIT_SUCCESS;
}

void to_big_float(char *str, big_float_t *num)
{
    memset(num, 0, sizeof(*num));

    if (str[0] == '-')
        num->sign = -1;
    else
        num->sign = 1;

    char *exp_pos = strpbrk(str, "eE");
    if (exp_pos != NULL)
        num->exponent = atoi(exp_pos + 1);
    else
        num->exponent = 0;

    char *dot_pos = strchr(str, '.');
    if (dot_pos != NULL)
    {
        if (exp_pos != NULL)
            num->exponent -= (exp_pos - dot_pos - 1);
        else
            num->exponent -= (strchr(str, '\0') - dot_pos - 1);
    }

    char *curr_p;
    if (exp_pos != NULL)
        curr_p = exp_pos - 1;
    else
        curr_p = strchr(str, '\0') - 1;

    size_t ind = 0;
    while (curr_p >= str)
    {
        if (isdigit(*curr_p))
            (num->mantissa)[ind++] = *curr_p - '0';
        curr_p--;
    } 
}

void big_int_to_big_float(big_int_t *int_num, big_float_t *float_num)
{
    float_num->sign = int_num->sign;
    memset(float_num->mantissa, 0, sizeof(float_num->mantissa));
    for (size_t i = 0; i < MAX_DIGITS_NUM; i++)
        float_num->mantissa[i] = int_num->digits[i];
    float_num->exponent = 0;
}

void shift_arr(int *arr, size_t arr_size, int shift)
{
    if (abs(shift) >= arr_size)
        for (size_t i = 0; i < arr_size; i++)
            arr[i] = 0;
    //<--
    else if (shift <= -1)
    {
        for (size_t i = 0; i < arr_size + shift; i++)
            arr[i] = arr[i - shift];
        for (size_t i = arr_size + shift; i < arr_size; i++)
            arr[i] = 0;
    }
    //-->
    else if (shift >= 1)
    {
        for (size_t i = arr_size - 1; i >= shift; i--)
            arr[i] = arr[i - shift];
        for (size_t i = 0; i <= shift - 1; i++)
            arr[i] = 0;
    }
}

int cmp_arr(int *a, int *b, size_t arr_size)
{
    int res = 0;
    size_t ind = arr_size - 1;
    while (ind > 0 && a[ind] == b[ind])
        ind--;

    if (a[ind] < b[ind])
        res = -1;
    if (a[ind] > b[ind])
        res = 1;

    return res;
}

void sub_arr(int *minuend, int *subtrahent, size_t arr_size)
{
    for (int i = arr_size - 1; i >= 0; i--)
    {
        if (minuend[i] >= subtrahent[i])
            minuend[i] -= subtrahent[i];
        else
        {
            minuend[i] += 10 - subtrahent[i];
            minuend[i + 1]--;
        }
    }
}

big_float_t division(big_float_t *dividend, big_float_t *divisor)
{
    big_float_t tmp_dividend = *dividend;
    big_float_t tmp_divisor = *divisor;

    tmp_dividend.exponent -= tmp_divisor.exponent;
    tmp_divisor.exponent = 0;

    int count;
    for (count = MAX_MANTISSA_LEN - 1; tmp_dividend.mantissa[count] == 0; count--);
    int dividend_size = count;

    for (count = MAX_MANTISSA_LEN - 1; tmp_divisor.mantissa[count] == 0; count--);
    int divisor_size = count;

    int diff = dividend_size - divisor_size;
    if (diff > 0)
    {
        shift_arr(tmp_divisor.mantissa, MAX_MANTISSA_LEN, diff);
        tmp_dividend.exponent += diff;
    }

    if (diff < 0)
    {
        shift_arr(tmp_dividend.mantissa, MAX_MANTISSA_LEN, -diff);
        tmp_dividend.exponent += diff;
    }

    int cmp_res;

    cmp_res = cmp_arr(tmp_dividend.mantissa, tmp_divisor.mantissa, MAX_MANTISSA_LEN);
    if (cmp_res == -1)
    {
        shift_arr(tmp_dividend.mantissa, MAX_MANTISSA_LEN, 1);
        tmp_dividend.exponent--;
    }

    big_float_t result;
    result.sign = tmp_dividend.sign * tmp_divisor.sign;
    memset(&result.mantissa, 0, sizeof(result.mantissa));
    result.exponent = tmp_dividend.exponent;

    int ind = MAX_MANTISSA_LEN - 1;
    int curr_cmp_res; 
    int is_zero = 1;

    while (ind >= 0)
    {
        cmp_res = cmp_arr(tmp_dividend.mantissa, tmp_divisor.mantissa, MAX_MANTISSA_LEN);

        if (cmp_res == 0)
        {
            (result.mantissa)[ind] = 1;
            break;
        }
        else if (cmp_res == 1)
        {
            is_zero = 0;
            count = 0;
            curr_cmp_res = cmp_arr(tmp_dividend.mantissa, tmp_divisor.mantissa, MAX_MANTISSA_LEN);
            while (curr_cmp_res != -1)
            {
                if (curr_cmp_res == 0)
                {
                    count++;
                    break;
                }

                sub_arr(tmp_dividend.mantissa, tmp_divisor.mantissa, MAX_MANTISSA_LEN);
                count++;
                curr_cmp_res = cmp_arr(tmp_dividend.mantissa, tmp_divisor.mantissa, MAX_MANTISSA_LEN);
            }

            (result.mantissa)[ind] = count;
            ind--;
            if (curr_cmp_res == 0)
                break;
        }
        else
        {
            if (is_zero)
            {

                (result.mantissa)[ind] = 0;
                ind--;
            }
            is_zero = 1;
            shift_arr(tmp_dividend.mantissa, MAX_MANTISSA_LEN, 1);
        }
    }

    size_t shift = 0;
    for (; shift < MAX_DIGITS_NUM && result.mantissa[shift] == 0; shift++);
    shift_arr(result.mantissa, MAX_MANTISSA_LEN, -shift);

    result.exponent++;

    return result;
}

void round_big_float(big_float_t *num, size_t round_digit)
{
    size_t non_zero_ind = MAX_MANTISSA_LEN - 1;
    for (;non_zero_ind > 0 && num->mantissa[non_zero_ind] == 0; non_zero_ind--);

    size_t round_ind = non_zero_ind - round_digit + 1;
    if (round_ind > 0 && (num->mantissa)[round_ind - 1] >= 5)
    {
        (num->mantissa)[round_ind]++;
        for (size_t i = round_ind; i < MAX_DIGITS_NUM && (num->mantissa)[i] > 9; i++)
        {
                num->mantissa[i] -= 10;
                num->mantissa[i + 1]++;
        }
        shift_arr(num->mantissa, MAX_MANTISSA_LEN, -round_ind);
    }
    num->mantissa[round_ind - 1] = 0;
}

void printf_normalized(big_float_t *num)
{
    if (num->sign == -1)
        printf("-0.");
    else if(num->sign == 1)
        printf("+0.");

    int end_ind = 0;
    for (; end_ind < MAX_DIGITS_NUM - 2 && num->mantissa[end_ind] == 0; end_ind++);

    int beg_ind = MAX_DIGITS_NUM - 1;
    for (; beg_ind > end_ind && num->mantissa[beg_ind] == 0; beg_ind--);

    int curr_ind = beg_ind;
    while (curr_ind >= end_ind && (beg_ind - curr_ind) < MAX_DIGITS_IO_NUM)
    {
        printf("%d", num->mantissa[curr_ind]);
        curr_ind--;
    }

    printf("e");
    if (num->exponent > 0)
        printf("+");
    printf("%d", num->exponent);
}
