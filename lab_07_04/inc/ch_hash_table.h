#ifndef CH_HASH_TABLE_H__
#define CH_HASH_TABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

#define MAX_COLLIZION_CH 10

typedef struct
{
    size_t (*hash_func)(char *data, size_t size);
    size_t size;
    size_t elem_count;
    char **data;
} ch_hash_table_t;

void ch_hash_create(ch_hash_table_t *table, size_t size, size_t hash_func(char *data, size_t size));
void ch_hash_destroy(ch_hash_table_t *table);
void ch_hash_restructure(ch_hash_table_t *table);
void ch_hash_insert(ch_hash_table_t *table, char *data);
char *ch_hash_search(ch_hash_table_t *table, char *data);
int ch_hash_delete(ch_hash_table_t *table, char *data);
void ch_hash_fprint(FILE *file, ch_hash_table_t *table);

void ch_hash_delete_by_smb(ch_hash_table_t *table, char smb);

#endif //CH_HASH_TABLE_H__
