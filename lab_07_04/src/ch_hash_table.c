#include "ch_hash_table.h"

void ch_hash_create(ch_hash_table_t *table, size_t size, size_t hash_func(char *data, size_t size))
{
    table->size = size;
    table->elem_count = 0;
    table->hash_func = hash_func;
    table->data = calloc(size, sizeof(char *));
}

void ch_hash_destroy(ch_hash_table_t *table)
{
    for (size_t i = 0; i < table->size; i++)
        if (table->data[i])
            free(table->data[i]);
    free(table->data);
    table->size = 0;
}

static int is_prime(size_t num)
{
    if (num <= 1)
        return 0;
    if (num <= 3)
        return 1;

    if (num % 2 == 0 || num % 3 == 0)
        return 0;

    for (size_t i = 5; i * i <= num; i+=6)
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;

    return 1;
}

static size_t next_prime(size_t num)
{
    if (num <= 1)
        return 2;

    size_t next_prime = num + 1;

    while (!is_prime(next_prime))
        next_prime++;

    return next_prime;
}

void ch_hash_restructure(ch_hash_table_t *table)
{
    size_t new_size = next_prime(table->size * 1.5);
    char **new_data = calloc(new_size, sizeof(char *));
    ch_hash_table_t new_table = {
        .size = new_size,
        .elem_count = 0,
        .hash_func = table->hash_func,
        .data = new_data,
    };

    for (size_t i = 0; i < table->size; i++)
        if (table->data[i])
            ch_hash_insert(&new_table, table->data[i]);

    ch_hash_destroy(table);
    *table = new_table;
}

void ch_hash_insert(ch_hash_table_t *table, char *data)
{
    size_t ind = table->hash_func(data, table->size);

    int flag = 1;
    if (!table->data[ind]) 
    {
        table->data[ind] = strdup(data);
        flag = 0;
    } 
    else 
        for (size_t i = (ind + 1) % table->size, cnt = 0; cnt < MAX_COLLIZION_CH && flag; i = (i + 1) % table->size, cnt++) 
            if (!table->data[i])
            {
                table->data[i] = strdup(data);
                flag = 0;
            }

    if (flag)
    {
        ch_hash_restructure(table);
        ch_hash_insert(table, data);
    }
    else
        table->elem_count++;
}

char *ch_hash_search(ch_hash_table_t *table, char *data)
{
    size_t ind = table->hash_func(data, table->size);

    if (table->data[ind] && strcmp(table->data[ind], data) == 0)
        return table->data[ind];

    for (size_t i = (ind + 1) % table->size, cnt = 0; cnt < MAX_COLLIZION_CH; i = (i + 1) % table->size, cnt++)
        if (table->data[i] && strcmp(table->data[i], data) == 0)
            return table->data[i];

    return NULL;
}

int ch_hash_delete(ch_hash_table_t *table, char *data)
{
    int flag = 0;
    size_t ind = table->hash_func(data, table->size);

    if (table->data[ind] && strcmp(table->data[ind], data) == 0) 
    {
        free(table->data[ind]);
        table->data[ind] = NULL;
        table->elem_count--;
        flag = 1;
    }
    else 
        for (size_t i = (ind + 1) % table->size, cnt = 0; cnt < MAX_COLLIZION_CH && !flag; i = (i + 1) % table->size, cnt++) 
        {
            if (table->data[i] && strcmp(table->data[i], data) == 0) 
            {
                free(table->data[i]);
                table->data[i] = NULL;
                table->elem_count--;
                flag = 1;
            }
        }

    return !flag;
}

void ch_hash_fprint(FILE *file, ch_hash_table_t *table)
{
    fprintf(file, "Таблица с закрытым хэшированием:\n");
    fprintf(file, "------+\n");

    for (size_t i = 0; i < table->size; i++)
        if (table->data[i])
            fprintf(file, "%6zu| %s\n", i, table->data[i]);
        else
            fprintf(file, "%6zu|\n", i);

    fprintf(file, "------+\n");
}

void ch_hash_delete_by_smb(ch_hash_table_t *table, char smb)
{
    for (size_t i = 0; i < table->size; i++)
    {
        if (table->data[i] && table->data[i][0] == smb)
        {
            free(table->data[i]);
            table->data[i] = NULL;
            table->elem_count--;
        }
    }
}
