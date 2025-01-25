#include "oh_hash_table.h"

void oh_hash_create(oh_hash_table_t *table, size_t size, size_t hash_func(char *data, size_t size))
{
    table->size = size;
    table->elem_count = 0;
    table->hash_func = hash_func;
    table->data = calloc(size, sizeof(*table->data));
}

static void free_list(hash_node_t *list)
{
    for (hash_node_t *curr = list; curr;) 
    {
        hash_node_t *tmp = curr->next;
        free(curr->data);
        free(curr);
        curr = tmp;
    }
}

void oh_hash_destroy(oh_hash_table_t *table)
{
    for (size_t i = 0; i < table->size; i++)
        if (table->data[i]) 
            free_list(table->data[i]);

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

void oh_hash_restructure(oh_hash_table_t *table)
{
    size_t new_size = next_prime(table->size * 1.5);
    hash_node_t **new_data = calloc(new_size, sizeof(*new_data));
    oh_hash_table_t new_table = {
        .size = new_size,
        .elem_count = 0,
        .hash_func = table->hash_func,
        .data = new_data,
    };

    for (size_t i = 0; i < table->size; i++)
        for (hash_node_t *curr = table->data[i]; curr; curr = curr->next)
            oh_hash_insert(&new_table, curr->data);

    oh_hash_destroy(table);
    *table = new_table;
}

void oh_hash_insert(oh_hash_table_t *table, char *data)
{
    size_t index = table->hash_func(data, table->size);
    hash_node_t *new_node = malloc(sizeof(*new_node));
    new_node->index = 0;
    new_node->data = strdup(data);
    new_node->next = NULL;

    if (!table->data[index]) 
        table->data[index] = new_node;
    else 
    {
        new_node->index++;
        hash_node_t *curr;
        for (curr = table->data[index]; curr->next != NULL; curr = curr->next, new_node->index++);
        curr->next = new_node;
    }

    if (new_node->index >= MAX_COLLIZION_OH) 
        oh_hash_restructure(table);
    else
        table->elem_count++;
}

hash_node_t *oh_hash_search(oh_hash_table_t *table, char *data) 
{
    size_t index = table->hash_func(data, table->size);

    for (hash_node_t *curr = table->data[index]; curr; curr = curr->next)
        if (strcmp(curr->data, data) == 0)
            return curr;

    return NULL;
}

int oh_hash_delete(oh_hash_table_t *table, char *data)
{
    int flag = 0;
    size_t index = table->hash_func(data, table->size);
    if (table->data[index]) 
    {
        if (!strcmp(table->data[index]->data, data)) 
        {
            hash_node_t *tmp = table->data[index]->next;
            free(table->data[index]->data);
            free(table->data[index]);
            table->data[index] = tmp;
            table->elem_count--;
            flag = 1;
        } 
        else 
        {
            hash_node_t *prev = NULL;
            for (hash_node_t *curr = table->data[index]->next; curr != NULL && !flag; curr = curr->next) 
            {
                if (!strcmp(curr->data, data)) 
                {
                    if (!prev) table->data[index] = curr->next;
                    else prev->next = curr->next;
                    table->elem_count--;
                    free(curr->data);
                    free(curr);
                    flag = 1;
                } 
                else
                    prev = curr;
            }
        }
    }

    return !flag;
}

void oh_hash_fprint(FILE *file, oh_hash_table_t *table)
{
    fprintf(file, "Таблица с открытым хэшированием:\n");
    fprintf(file, "------+\n");

    for (size_t i = 0; i < table->size; i++)
    {
        if (table->data[i]) 
        {
            fprintf(file, "%6zu| %s", i, table->data[i]->data);
            for (hash_node_t *curr = table->data[i]->next; curr; curr = curr->next) 
                fprintf(file, "%6s| %s", " ", curr->data);
            fprintf(file, "\n");
        } 
        else 
            fprintf(file, "%6zu|\n", i);
    }

    fprintf(file, "------+\n");
}

void oh_hash_delete_by_smb(oh_hash_table_t *table, char smb) 
{
    if (!table) 
        return;

    for (size_t i = 0; i < table->size; i++) 
    {
        hash_node_t *curr = table->data[i];
        hash_node_t *prev = NULL;

        while (curr) 
        {
            if (curr->data[0] == smb) 
            {
                hash_node_t *to_delete = curr;

                if (prev == NULL) 
                    table->data[i] = curr->next;
                else 
                    prev->next = curr->next;

                curr = curr->next;
                free(to_delete->data);
                free(to_delete);
                table->elem_count--;
            } 
            else 
            {
                prev = curr;
                curr = curr->next;
            }
        }
    }
}
