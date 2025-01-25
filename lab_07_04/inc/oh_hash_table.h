#ifndef oh_HASH_TABLE_H__
#define oh_HASH_TABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

#define MAX_COLLIZION_OH 4

typedef struct hash_node_t
{
    char *data;
    size_t index;
    struct hash_node_t *next;
} hash_node_t;

typedef struct
{
    size_t (*hash_func)(char *data, size_t size);
    size_t size;
    size_t elem_count;
    hash_node_t **data;
} oh_hash_table_t;

void oh_hash_create(oh_hash_table_t *table, size_t size, size_t hash_func(char *data, size_t size));
void oh_hash_destroy(oh_hash_table_t *table);
void oh_hash_restructure(oh_hash_table_t *table);
void oh_hash_insert(oh_hash_table_t *table, char *data);
hash_node_t *oh_hash_search(oh_hash_table_t *table, char *data);
int oh_hash_delete(oh_hash_table_t *table, char *data);
void oh_hash_fprint(FILE *file, oh_hash_table_t *table);

void oh_hash_delete_by_smb(oh_hash_table_t *table, char smb);

#endif //oh_HASH_TABLE_H__
