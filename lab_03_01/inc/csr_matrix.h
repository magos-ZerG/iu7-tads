#ifndef CSR_MATRIX_H__
#define CSR_MATRIX_H__

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define CSR_MATRIX_ALLOCATION_ERROR 1
#define CSR_MATRIX_INVALID_ROWS_NUM 2
#define CSR_MATRIX_INVALID_COLUMNS_NUM 3
#define CSR_MATRIX_INVALID_POINTS_NUM 4
#define CSR_MATRIX_BUFF_ALLOCATION_ERROR 5
#define CSR_MATRIX_INVALID_ELEM 6
#define CSR_MATRIX_OK 0

typedef struct 
{
    size_t columns;
    vector_t data;
    vector_t columns_inds;
    vector_t rows_ptrs;
} *csr_matrix_t;

#define ASSERT_IS_CSR_MATRIX(csr_matrix)                                          \
    do                                                                            \
    {                                                                             \
        assert((csr_matrix));                                                     \
        assert((csr_matrix)->columns > 0);                                        \
        ASSERT_IS_VECTOR(((csr_matrix)->data));                                   \
        ASSERT_IS_VECTOR(((csr_matrix)->columns_inds));                           \
        ASSERT_IS_VECTOR(((csr_matrix)->rows_ptrs));                              \
        assert(((csr_matrix)->data)->size == ((csr_matrix)->columns_inds)->size); \
    } while (0)

int csr_matrix_create(csr_matrix_t *csr_matrix, size_t elemsize, size_t rows, size_t columns);

void csr_matrix_destroy(csr_matrix_t *csr_matrix);

size_t csr_matrix_rows(csr_matrix_t matrix);

size_t csr_matrix_columns(csr_matrix_t matrix);

void *csr_matrix_at(csr_matrix_t csr_matrix, size_t i, size_t j);

int csr_matrix_put(csr_matrix_t *csr_matrix, void *data, size_t i, size_t j);

csr_matrix_t csr_matrix_sum(csr_matrix_t *csr_matrix1, csr_matrix_t *csr_matrix2, void (*sum)(const void *, const void *, const void *));

int csr_matrix_fscan(FILE *file, csr_matrix_t *csr_matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const));

int csr_matrix_user_fscan(csr_matrix_t *csr_matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const));

int csr_matrix_fprint(FILE *file, csr_matrix_t csr_matrix, int (*fprint_elem)(FILE * const, void * const));

int csr_matrix_copy(csr_matrix_t *dst, csr_matrix_t *src);

#endif //CSR_MATRIX_H__