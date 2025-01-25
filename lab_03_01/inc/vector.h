#ifndef VECTOR_H__
#define VECTOR_H__
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define VECTOR_INIT_CAPACITY 32
#define VECTOR_EXPANCION_COEFF 2

#define VECTOR_ALLOCATION_ERROR 1
#define VECTOR_OK 0

typedef struct 
{
    size_t elem_size;
    size_t capacity;
    size_t size;
    void *data;
} *vector_t;

#define ASSERT_IS_VECTOR(vector)                      \
    do                                                \
    {                                                 \
        assert((vector));                             \
        assert((vector)->elem_size != 0);             \
        assert((vector)->capacity > 0);               \
        assert((vector)->size <= (vector)->capacity); \
        assert((vector)->data);                       \
    } while (0)

int vector_create(vector_t *vector, size_t elemsize, size_t *init_capacity);

void vector_destroy(vector_t *vector);

size_t vector_size(vector_t vector);

int vector_empty(vector_t vector);

void *vector_at(vector_t vector, size_t ind);

int vector_shrink_to_fit(vector_t *vector);

int vector_push_back(vector_t *vector, void *data, size_t data_size);

void vector_pop_back(vector_t *vector);

int vector_insert(vector_t *vector, void *data, size_t ind);

int vector_copy(vector_t *dst, vector_t *src);
 
#endif // VECTOR_H__
