#include "vector.h"

int vector_create(vector_t *vector, size_t elemsize, size_t *init_capacity) 
{
    vector_t tmp = malloc(sizeof(**vector));

    if (tmp == NULL)
        return VECTOR_ALLOCATION_ERROR;

    tmp->elem_size = elemsize;
    if (init_capacity)
        tmp->capacity = (size_t)init_capacity;
    else
        tmp->capacity = VECTOR_INIT_CAPACITY;
    tmp->size = 0;

    tmp->data = malloc(elemsize * tmp->capacity);

    if (tmp->data == NULL)
    {
        free(tmp);
        return VECTOR_ALLOCATION_ERROR;
    }

    (*vector)=tmp;

    ASSERT_IS_VECTOR(*vector); 

    return VECTOR_OK; 
}

void vector_destroy(vector_t *vector)
{
    if (vector == NULL || *vector == NULL)
        return;
    ASSERT_IS_VECTOR(*vector);

    free((*vector)->data);
    free(*vector);
    (*vector) = NULL;
}

size_t vector_size(vector_t vector)
{
    ASSERT_IS_VECTOR(vector);

    return vector->size;
}

int vector_empty(vector_t vector)
{
    ASSERT_IS_VECTOR(vector);

    return vector->size == 0;
}

void *vector_at(vector_t vector, size_t ind)
{
    ASSERT_IS_VECTOR(vector);
    assert(ind < vector->size);

    return (void *)((char *)(vector->data) + (vector->elem_size) * ind);
}

int vector_shrink_to_fit(vector_t *vector)
{
    assert(vector);
    ASSERT_IS_VECTOR(*vector);

    if ((*vector)->size != (*vector)->capacity)
    {
        void *tmp = realloc((*vector)->data, (*vector)->elem_size * (*vector)->size);
        if (tmp == NULL)
            return VECTOR_ALLOCATION_ERROR;
        ((*vector)->data) = tmp;
        ((*vector)->capacity) = ((*vector)->size);
    }

    return VECTOR_OK;
}

int vector_push_back(vector_t *vector, void *data, size_t data_size)
{
    assert(vector);
    ASSERT_IS_VECTOR(*vector);

    while ((*vector)->size + data_size > (*vector)->capacity)
    {
        void *tmp = realloc((*vector)->data, (*vector)->elem_size * (*vector)->capacity * VECTOR_EXPANCION_COEFF);
        if (tmp == NULL)
            return VECTOR_ALLOCATION_ERROR;
        ((*vector)->data) = tmp;
        ((*vector)->capacity) *= VECTOR_EXPANCION_COEFF;
    }

    memcpy((char *)((*vector)->data) + (*vector)->size * (*vector)->elem_size, data, data_size * (*vector)->elem_size);
    ((*vector)->size) += data_size;

    return VECTOR_OK;
}

void vector_pop_back(vector_t *vector)
{
    assert(vector);
    ASSERT_IS_VECTOR(*vector);
    assert((*vector)->size > 0);

    ((*vector)->size)--;
}

int vector_insert(vector_t *vector, void *data, size_t ind)
{
    assert(vector);
    ASSERT_IS_VECTOR(*vector);
    assert(ind <= (*vector)->size);

    if (vector_push_back(vector, data, (size_t)1) != VECTOR_OK)
        return VECTOR_ALLOCATION_ERROR;

    for (size_t i = (*vector)->size - 1; i > ind; i--)
        memcpy(vector_at(*vector, i), vector_at(*vector, i - 1), (*vector)->elem_size);

    memcpy(vector_at(*vector, ind), data, (*vector)->elem_size);

    return VECTOR_OK;
}

int vector_copy(vector_t *dst, vector_t *src)
{
    void *buff = realloc((*dst)->data, (*src)->elem_size * (*src)->capacity);
    if (buff == NULL)
        return VECTOR_ALLOCATION_ERROR;
    memcpy(buff, (*src)->data, (*src)->elem_size * (*src)->size);
    (*dst)->data = buff;

    (*dst)->elem_size = (*src)->elem_size; 
    (*dst)->capacity = (*src)->capacity; 
    (*dst)->size = (*src)->size; 

    return VECTOR_OK;
}
