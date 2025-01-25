#include "matrix.h"

int matrix_create(matrix_t *matrix, size_t elemsize, size_t rows, size_t columns)
{
    matrix_t tmp = malloc(sizeof(**matrix));

    if (tmp == NULL)
        return MATRIX_ALLOCATION_ERROR;

    tmp->elem_size = elemsize;
    tmp->rows = rows;
    tmp->columns = columns;

    tmp->data = malloc(sizeof(void *) * rows);

    if (tmp->data == NULL)
    {
        free(tmp);
        return MATRIX_ALLOCATION_ERROR;
    }

    for (size_t i = 0; i < rows; i++)
    {
        (tmp->data)[i] = malloc(elemsize * columns);
        if ((tmp->data)[i] == NULL)
        {
            for (size_t j = 0; j <= i; j++)
                free((tmp->data)[i]);
            free(tmp->data);
            free(tmp);
            return MATRIX_ALLOCATION_ERROR;
        }
    }

    (*matrix) = tmp;

    ASSERT_IS_MATRIX(*matrix);

    return MATRIX_OK;
}

void matrix_destroy(matrix_t *matrix)
{
    if (matrix == NULL || *matrix == NULL)
        return;
    ASSERT_IS_MATRIX(*matrix);
    
    for (size_t i = 0; i < (*matrix)->rows; i++)
        free(((*matrix)->data)[i]); 

    free((*matrix)->data);
    free(*matrix);
    (*matrix) = NULL;
}

size_t matrix_rows(matrix_t matrix)
{
    ASSERT_IS_MATRIX(matrix);

    return matrix->rows;
}

size_t matrix_columns(matrix_t matrix)
{
    ASSERT_IS_MATRIX(matrix);

    return matrix->columns;
}

void *matrix_at(matrix_t matrix, size_t i, size_t j)
{
    ASSERT_IS_MATRIX(matrix);
    assert(i < matrix->rows);
    assert(j < matrix->columns);

    return (void *)((char *)(matrix->data)[i] + matrix->elem_size * j);
}

matrix_t matrix_sum(matrix_t *matrix1, matrix_t *matrix2, void (*sum)(const void *, const void *, const void *))
{
    assert(matrix1);
    ASSERT_IS_MATRIX(*matrix1);
    assert(matrix2);
    ASSERT_IS_MATRIX(*matrix2);
    assert((*matrix1)->elem_size == (*matrix2)->elem_size);
    assert((*matrix1)->rows == (*matrix2)->rows);
    assert((*matrix1)->columns == (*matrix2)->columns);
    
    matrix_t res;
    int rc = matrix_create(&res, (*matrix1)->elem_size, (*matrix1)->rows, (*matrix1)->columns);

    if (rc != MATRIX_OK)
        return NULL;

    void *tmp = malloc(res->elem_size);
    if (tmp == NULL)
    {
        matrix_destroy(&res);
        return NULL;
    }

    for (size_t i = 0; i < res->rows; i++)
        for (size_t j = 0; j < res->columns; j++)
        {
            sum(tmp, matrix_at(*matrix1, i, j), matrix_at(*matrix2, i, j));
            memcpy(matrix_at(res, i, j), tmp, res->elem_size);
        }

    free(tmp);

    return res;
}

int matrix_fscan(FILE *file, matrix_t *matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const))
{
    size_t rows;
    if (fscanf(file, "%zu", &rows) != 1 || rows <= 0)
        return MATRIX_INVALID_ROWS_NUM;

    size_t columns;
    if (fscanf(file, "%zu", &columns) != 1 || columns <= 0)
        return MATRIX_INVALID_COLUMNS_NUM;

    int rc = matrix_create(matrix, elem_size, rows, columns);
    if (rc != MATRIX_OK)
        return MATRIX_ALLOCATION_ERROR;

    void *tmp = malloc(elem_size);
    if (tmp == NULL)
    {   
        matrix_destroy(matrix);
        return MATRIX_BUFF_ALLOCATION_ERROR;
    }

    for (size_t i = 0; i < (*matrix)->rows; i++)
        for (size_t j = 0; j < (*matrix)->columns; j++)
        {
            if (fscan_elem(file, tmp) != EXIT_SUCCESS)
            {
                matrix_destroy(matrix);
                free(tmp);
                return MATRIX_INVALID_ELEM;
            }
            memcpy(matrix_at(*matrix, i, j), tmp, elem_size);
        }
    
    free(tmp);

    return MATRIX_OK;   
}

int matrix_user_fscan(matrix_t *matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const))
{
    size_t rows;
    printf("Введите число строк:\n");
    while (scanf("%zu", &rows) != 1 || getchar() != '\n' || rows <= 0)
    {
        printf("Невалидное число строк.\n");
        while (getchar() != '\n');
    }

    size_t columns;
    printf("Введите число столбцов:\n");
    while (scanf("%zu", &columns) != 1 || getchar() != '\n' || columns <= 0)
    {
        printf("Невалидное число столбцов.\n");
        while (getchar() != '\n');
    }

    if (*matrix != NULL)
        matrix_destroy(matrix);
    int rc = matrix_create(matrix, elem_size, rows, columns);
    if (rc != MATRIX_OK)
        return MATRIX_ALLOCATION_ERROR;

    void *tmp = malloc(elem_size);
    if (tmp == NULL)
    {   
        matrix_destroy(matrix);
        return MATRIX_BUFF_ALLOCATION_ERROR;
    }

    char buff;

    printf("Введите матрицу:\n");

    for (size_t i = 0; i < (*matrix)->rows; i++)
        for (size_t j = 0; j < (*matrix)->columns; j++)
        {
            while (fscan_elem(stdin, tmp) != EXIT_SUCCESS || ((buff = getchar()) != '\n' && buff != ' '))
            {
                printf("Невалидный элемент.\n");
                while (getchar() != '\n');
            }
            memcpy(matrix_at(*matrix, i, j), tmp, elem_size);
        }
    
    free(tmp);

    return MATRIX_OK;
}

int matrix_fprint(FILE *file, matrix_t matrix, int (*fprint_elem)(FILE * const, void * const))
{
    assert(matrix);
    ASSERT_IS_MATRIX(matrix);

    for (size_t i = 0; i < matrix->rows; i++)
    {
        for (size_t j = 0; j < matrix->columns; j++)
        {
            fprint_elem(file, matrix_at(matrix, i, j));
            fprintf(file, " ");
        }
        fprintf(file, "\n");
    }

    return MATRIX_OK;   
}
