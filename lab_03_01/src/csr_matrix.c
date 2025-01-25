#include "csr_matrix.h"

int csr_matrix_create(csr_matrix_t *csr_matrix, size_t elemsize, size_t rows, size_t columns)
{
    csr_matrix_t tmp = malloc(sizeof(**csr_matrix));

    if (tmp == NULL)
        return CSR_MATRIX_ALLOCATION_ERROR;

    tmp->columns = columns;

    int rc = vector_create(&(tmp->data), elemsize, NULL);

    if (rc != VECTOR_OK)
    {
        free(tmp);
        return CSR_MATRIX_ALLOCATION_ERROR;
    }

    rc = vector_create(&(tmp->columns_inds), sizeof(size_t), NULL);

    if (rc != VECTOR_OK)
    {
        vector_destroy(&(tmp->data));
        free(tmp);
        return CSR_MATRIX_ALLOCATION_ERROR;
    }

    rc = vector_create(&(tmp->rows_ptrs), sizeof(size_t), (size_t *)(rows + (size_t)1));

    if (rc != VECTOR_OK)
    {
        vector_destroy(&(tmp->data));
        vector_destroy(&(tmp->columns_inds));
        free(tmp);
        return CSR_MATRIX_ALLOCATION_ERROR;
    }

    size_t zero = 0;
    for (size_t i = 0; i < rows + 1; i++)
        vector_push_back(&(tmp->rows_ptrs), &zero, 1);

    (*csr_matrix) = tmp;

    ASSERT_IS_CSR_MATRIX(*csr_matrix);

    return CSR_MATRIX_OK;
}

void csr_matrix_destroy(csr_matrix_t *csr_matrix)
{
    if (csr_matrix == NULL || *csr_matrix == NULL)
        return;
    ASSERT_IS_CSR_MATRIX(*csr_matrix);

    vector_destroy(&((*csr_matrix)->data));
    vector_destroy(&((*csr_matrix)->columns_inds));
    vector_destroy(&((*csr_matrix)->rows_ptrs));
    free(*csr_matrix);

    (*csr_matrix) = NULL;
}

size_t csr_matrix_rows(csr_matrix_t csr_matrix)
{
    ASSERT_IS_CSR_MATRIX(csr_matrix);

    return ((csr_matrix->rows_ptrs)->size) - 1; 
}

size_t csr_matrix_columns(csr_matrix_t csr_matrix)
{
    ASSERT_IS_CSR_MATRIX(csr_matrix);

    return csr_matrix->columns;
}

void *csr_matrix_at(csr_matrix_t csr_matrix, size_t i, size_t j)
{
    ASSERT_IS_CSR_MATRIX(csr_matrix);
    assert(i < csr_matrix_rows(csr_matrix));
    assert(j < csr_matrix->columns);

    size_t row_ptrb = *(size_t *)vector_at(csr_matrix->rows_ptrs, i);
    size_t row_ptre = *(size_t *)vector_at(csr_matrix->rows_ptrs, i + 1);

    size_t low, mid, high;
    size_t curr_elem = j + 1;
    low = 0;
    high = row_ptre - row_ptrb;
    while (low < high)
    {
        mid = low + (high - low) / 2;
        curr_elem = *(size_t *)vector_at(csr_matrix->columns_inds, row_ptrb + mid);

        if (curr_elem > j)
            high = mid;
        else if (curr_elem < j)
            low = mid + 1;
        else
            break;
    }
    
    if (curr_elem == j)
        return vector_at(csr_matrix->data, row_ptrb + mid);
    return NULL;
}

int csr_matrix_put(csr_matrix_t *csr_matrix, void *data, size_t i, size_t j)
{
    assert(csr_matrix);    
    ASSERT_IS_CSR_MATRIX(*csr_matrix);

    csr_matrix_t buff;
    size_t new_rows;
    if (i > ((*csr_matrix)->rows_ptrs)->size - 1)
        new_rows = i;
    else
        new_rows = ((*csr_matrix)->rows_ptrs)->size - 1;

    int rc = csr_matrix_create(&buff, (*csr_matrix)->data->elem_size, new_rows, (*csr_matrix)->columns);
    if (rc != CSR_MATRIX_OK)
        return CSR_MATRIX_ALLOCATION_ERROR;

    for (size_t i = 0; i < (*csr_matrix)->data->size; i++)
        rc = vector_push_back(&(buff->data), vector_at((*csr_matrix)->data, i), 1);
    for (size_t i = 0; i < (*csr_matrix)->columns_inds->size; i++)
        rc = vector_push_back(&(buff->columns_inds), vector_at((*csr_matrix)->columns_inds, i), 1);
    memcpy(((buff->rows_ptrs)->data), (*csr_matrix)->rows_ptrs->data, (*csr_matrix)->rows_ptrs->size * (*csr_matrix)->rows_ptrs->elem_size);
    buff->rows_ptrs->size = (*csr_matrix)->rows_ptrs->size;
    
    size_t last_ptr = (*csr_matrix)->data->size;
    if (i >= ((*csr_matrix)->rows_ptrs)->size - 1)
    {
        for (size_t k = (((buff->rows_ptrs)->size) - 1); k < i + 1; k++)
        {
            rc = vector_push_back(&(buff->rows_ptrs), &last_ptr, 1); 
            if (rc != VECTOR_OK)
            {
                csr_matrix_destroy(&buff);
                return CSR_MATRIX_ALLOCATION_ERROR;
            }
        }
    }

    if (j >= (*csr_matrix)->columns)
        buff->columns = j + 1;

    size_t row_ptrb = *(size_t *)vector_at(buff->rows_ptrs, i);
    size_t row_ptre = *(size_t *)vector_at(buff->rows_ptrs, i + 1);

    void *res_elem = csr_matrix_at(buff, i, j);
    if (res_elem != NULL)
    {
        memcpy(res_elem, data, (buff->data)->elem_size);
    }
    else
    {
        size_t insert_ind = row_ptrb;
        while (insert_ind < row_ptre && *(size_t *)vector_at(buff->columns_inds, insert_ind) < j)        
            insert_ind++;

        rc = vector_insert(&(buff->data), data, insert_ind);
        if (rc != VECTOR_OK)
        {        
            csr_matrix_destroy(&buff);
            return CSR_MATRIX_ALLOCATION_ERROR;
        }

        rc = vector_insert(&(buff->columns_inds), &j, insert_ind);
        if (rc != VECTOR_OK)        
        {        
            csr_matrix_destroy(&buff);
            return CSR_MATRIX_ALLOCATION_ERROR;
        }

        size_t tmp;
        for (size_t k = i + 1; k < buff->rows_ptrs->size; k++)    
        {
            tmp = *(size_t *)vector_at(buff->rows_ptrs, k) + 1;        
            memcpy(vector_at(buff->rows_ptrs, k), &tmp, sizeof(size_t));
        }
    }

    csr_matrix_destroy(csr_matrix);
    (*csr_matrix) = buff;

    return CSR_MATRIX_OK;
}

csr_matrix_t csr_matrix_sum(csr_matrix_t *csr_matrix1, csr_matrix_t *csr_matrix2, void (*sum)(const void *, const void *, const void *))
{
    assert(csr_matrix1);
    ASSERT_IS_CSR_MATRIX(*csr_matrix1);
    assert(csr_matrix2);
    ASSERT_IS_CSR_MATRIX(*csr_matrix2);
    assert((*csr_matrix1)->data->elem_size == (*csr_matrix2)->data->elem_size);
    assert((*csr_matrix1)->rows_ptrs->size == (*csr_matrix2)->rows_ptrs->size);
    assert((*csr_matrix1)->columns == (*csr_matrix2)->columns);

    csr_matrix_t res;
    int rc = csr_matrix_create(&res, (*csr_matrix1)->data->elem_size, ((*csr_matrix1)->rows_ptrs)->size - 1, (*csr_matrix1)->columns);

    if (rc != CSR_MATRIX_OK)
        return NULL;

    rc = csr_matrix_copy(&res, csr_matrix1);

    if (rc != CSR_MATRIX_OK)
    {
        csr_matrix_destroy(&res);
        return NULL;
    }

    void *tmp_elem = malloc(res->data->elem_size);

    if (tmp_elem == NULL)
    {
        csr_matrix_destroy(&res);
        return NULL;
    }

    size_t row_ptrb = 0;
    size_t row_ptre = 0;
    size_t curr_column;
    void *tmp;
    for (size_t i = 0; i < ((*csr_matrix2)->rows_ptrs)->size - 1; i++)
    {
        row_ptrb = row_ptre;
        row_ptre = *(size_t *)vector_at((*csr_matrix2)->rows_ptrs, i + 1);
        for (size_t j = row_ptrb; j < row_ptre; j++)
        {
            curr_column = *(size_t *)vector_at((*csr_matrix2)->columns_inds, j);
            tmp = csr_matrix_at(res, i, curr_column);
            if (tmp == NULL)
            {
                rc = csr_matrix_put(&res, csr_matrix_at(*csr_matrix2, i, curr_column), i, curr_column);
                if (rc != CSR_MATRIX_OK)
                {
                    csr_matrix_destroy(&res);
                    free(tmp_elem);
                    return NULL;
                }
            }
            else
            {
                sum(tmp_elem, tmp, csr_matrix_at(*csr_matrix2, i, curr_column));
                memcpy(tmp, tmp_elem, res->data->elem_size);
            }
        }
    }

    free(tmp_elem);

    return res;
}

int csr_matrix_fscan(FILE *file, csr_matrix_t *csr_matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const))
{
    size_t rows;
    if (fscanf(file, "%zu", &rows) != 1 || rows <= 0)
        return CSR_MATRIX_INVALID_ROWS_NUM;

    size_t columns;
    if (fscanf(file, "%zu", &columns) != 1 || columns <= 0)
        return CSR_MATRIX_INVALID_COLUMNS_NUM;

    size_t points_num;
    if (fscanf(file, "%zu", &points_num) != 1 || points_num <= 0 || points_num > rows * columns)
        return CSR_MATRIX_INVALID_POINTS_NUM;

    int rc = csr_matrix_create(csr_matrix, elem_size, rows, columns);

    void *tmp = malloc(elem_size);
    if (tmp == NULL)
    {
        csr_matrix_destroy(csr_matrix);
        return CSR_MATRIX_BUFF_ALLOCATION_ERROR;
    }

    size_t x, y;

    for (size_t i = 0; i < points_num; i++)
    {
        if (fscanf(file, "%zu%zu", &x, &y) != 2 || x >= rows || y >= columns)
        {
            csr_matrix_destroy(csr_matrix);
            free(tmp);
            return CSR_MATRIX_INVALID_ELEM;
        }

        if (fscan_elem(file, tmp) != EXIT_SUCCESS)
        {
            csr_matrix_destroy(csr_matrix);
            free(tmp);
            return CSR_MATRIX_INVALID_ELEM;
        }

        if (csr_matrix_at(*csr_matrix, x, y) != NULL)
        {
            csr_matrix_destroy(csr_matrix);
            free(tmp);
            return CSR_MATRIX_INVALID_ELEM;
        }
        else
        {
            rc = csr_matrix_put(csr_matrix, tmp, x, y);
            if (rc != CSR_MATRIX_OK)
            {
                csr_matrix_destroy(csr_matrix);
                free(tmp);
                return CSR_MATRIX_ALLOCATION_ERROR; 
            }
        }
    }

    free(tmp);

    return CSR_MATRIX_OK;
}

int csr_matrix_user_fscan(csr_matrix_t *csr_matrix, size_t elem_size, int (*fscan_elem)(FILE * const, void * const))
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

    size_t points_num;
    printf("Введите число ненулевых элементов матрицы:\n");
    while (scanf("%zu", &points_num) != 1 || getchar() != '\n' || points_num < 0 || points_num > rows * columns)
    {
        printf("Невалидное число ненулевых элементов матрицы.\n");
        while (getchar() != '\n');
    }

    if (*csr_matrix != NULL)
        csr_matrix_destroy(csr_matrix);
    int rc = csr_matrix_create(csr_matrix, elem_size, rows, columns);

    void *tmp = malloc(elem_size);
    if (tmp == NULL)
    {
        csr_matrix_destroy(csr_matrix);
        return CSR_MATRIX_BUFF_ALLOCATION_ERROR;
    }

    size_t x, y;
    size_t i = 0;
    char buff;

    if (points_num > 0)
        printf("Введите ненулевые точки в формате: <строка> <столбец> <значение>\n");
    while (i < points_num)
    {
        while (scanf("%zu%zu", &x, &y) != 2 || ((buff = getchar()) != '\n' && buff != ' ') || x >= rows || y >= columns || csr_matrix_at(*csr_matrix, x, y) != NULL)
        {
            printf("Невалидные координаты элемента.\n");
            while (getchar() != '\n');
        }

        while ((fscan_elem(stdin, tmp) != EXIT_SUCCESS) || getchar() != '\n')
        {
            printf("Невалидный элемент.\n");
            while (getchar() != '\n');
        }

        rc = csr_matrix_put(csr_matrix, tmp, x, y);
        if (rc != CSR_MATRIX_OK)
        {
            csr_matrix_destroy(csr_matrix);
            free(tmp);
            return CSR_MATRIX_ALLOCATION_ERROR; 
        }
        else
            i++;
    }

    free(tmp);

    return CSR_MATRIX_OK;
}

int csr_matrix_fprint(FILE *file, csr_matrix_t csr_matrix, int (*fprint_elem)(FILE * const, void * const))
{  
    for (size_t i = 0; i < (csr_matrix->data)->size; i++)
    {
        fprint_elem(file, vector_at(csr_matrix->data, i));
        fprintf(file, " ");
    }
    fprintf(file, "\n");

    for (size_t i = 0; i < csr_matrix->columns_inds->size; i++)
        fprintf(file, "%zu ", *(size_t *)vector_at(csr_matrix->columns_inds, i));
    fprintf(file, "\n");

    for (size_t i = 0; i < csr_matrix->rows_ptrs->size; i++)
        fprintf(file, "%zu ", *(size_t *)vector_at(csr_matrix->rows_ptrs, i));
    fprintf(file, "\n");

    return CSR_MATRIX_OK;
}

int csr_matrix_copy(csr_matrix_t *dst, csr_matrix_t *src)
{
    assert(dst);
    ASSERT_IS_CSR_MATRIX(*dst);
    assert(src);
    ASSERT_IS_CSR_MATRIX(*src);

    vector_t buff_data;
    int rc = vector_create(&buff_data, ((*src)->data)->elem_size, (size_t *)(((*src)->data)->capacity));
    if (rc != VECTOR_OK)
        return CSR_MATRIX_ALLOCATION_ERROR;
    buff_data->size = ((*src)->data)->size;
    memcpy(buff_data->data, ((*src)->data)->data, buff_data->size * buff_data->elem_size);

    vector_t buff_columns_inds;
    rc = vector_create(&buff_columns_inds, ((*src)->columns_inds)->elem_size, (size_t *)(((*src)->columns_inds)->capacity));
    if (rc != VECTOR_OK)
        return CSR_MATRIX_ALLOCATION_ERROR;
    buff_columns_inds->size = ((*src)->columns_inds)->size;
    memcpy(buff_columns_inds->data, ((*src)->columns_inds)->data, buff_columns_inds->size * buff_columns_inds->elem_size);

    vector_t buff_rows_prts;
    rc = vector_create(&buff_rows_prts, ((*src)->rows_ptrs)->elem_size, (size_t *)(((*src)->rows_ptrs)->capacity));
    if (rc != VECTOR_OK)
        return CSR_MATRIX_ALLOCATION_ERROR;
    buff_rows_prts->size = ((*src)->rows_ptrs)->size;
    memcpy(buff_rows_prts->data, ((*src)->rows_ptrs)->data, buff_rows_prts->size * buff_rows_prts->elem_size);

    (*dst)->columns = (*src)->columns;
    vector_destroy(&((*dst)->data));
    (*dst)->data = buff_data;
    vector_destroy(&((*dst)->columns_inds));
    (*dst)->columns_inds = buff_columns_inds;
    vector_destroy(&((*dst)->rows_ptrs));
    (*dst)->rows_ptrs = buff_rows_prts;

    return CSR_MATRIX_OK;
}
