#ifndef MATRIX_H__
#define MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MATRIX_ALLOCATION_ERROR 1
#define MATRIX_INVALID_ROWS_NUM 2
#define MATRIX_INVALID_COLUMNS_NUM 3
#define MATRIX_INVALID_ELEM 4
#define MATRIX_BUFF_ALLOCATION_ERROR 5
#define MATRIX_OK 0

typedef struct {
    size_t elem_size;
    size_t rows;
    size_t columns;
    void **data;
} *matrix_t;

#define ASSERT_IS_MATRIX(matrix)                    \
    do                                              \
    {                                               \
        assert((matrix));                           \
        assert((matrix)->elem_size > 0);            \
        assert((matrix)->rows > 0);                 \
        assert((matrix)->columns > 0);              \
        assert((matrix)->data);                     \
        for (size_t i = 0; i < (matrix)->rows; i++) \
            assert(((matrix)->data)[i]);            \
    } while (0)

int matrix_create(matrix_t *matrix, size_t elemsize, size_t rows, size_t columns);

void matrix_destroy(matrix_t *matrix);

size_t matrix_rows(matrix_t matrix);

size_t matrix_columns(matrix_t matrix);

void *matrix_at(matrix_t matrix, size_t i, size_t j);

matrix_t matrix_sum(matrix_t *matrix1, matrix_t *matrix2, void (*sum)(const void *, const void *, const void *));

int matrix_fscan(FILE *file, matrix_t *matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const));

int matrix_user_fscan(matrix_t *matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const));

int matrix_fprint(FILE *file, matrix_t matrix, int (*fprint_elem)(FILE * const, void * const));

#endif //MATRIX_H__
