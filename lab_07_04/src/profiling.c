#include "profiling.h"

static int cmp(void *word1, void *word2)
{
    return strcmp((char *)word1, (char *)word2);
}

static size_t hash_func(char *key, size_t size)
{
    size_t hash = 0;
    char *curr = key;

    while (*curr)
    {
        if ((curr - 1) > key)
            hash += (*(curr - 1) - 'a' + 1) * *curr;
        else
            hash += *curr;
        curr++;
    }
    hash = (hash * strlen(key)) % size;

    return hash;
}

int time_measurement(void)
{
    tree_node_t *tree = NULL;
    tree_node_t *node = NULL;
    avl_tree_node_t *avl_tree = NULL;
    avl_tree_node_t *avl_node = NULL;

    oh_hash_table_t oh_hash_table;
    ch_hash_table_t ch_hash_table;

    size_t sizes[5] = {100, 200, 300, 400, 500};

    char *cmd = (char *)malloc(63);
    size_t wordsize = 5;

    char *data_filename = (char *)malloc(35);
    FILE *data_file;

    long long deletion_time[5][4];
    memset(&deletion_time[0][0], 0, sizeof(long long) * 20);

    long long search_time[5][4];
    memset(&search_time[0][0], 0, sizeof(long long) * 20);

    struct timespec start, end;

    for (size_t i = 0; i < 5; i++)
    {
        snprintf(cmd, 63, "python3 ./tools/gen_words.py --wordsnum %zu --wordsize %zu", sizes[i], wordsize);
        system(cmd);
    }

    char *data = NULL;
    size_t data_len = 0;

    for (size_t i = 0; i < 5; i++)
    {
        snprintf(data_filename, 38, "./out/test_%zu_random.txt", sizes[i]);
        for (size_t k = 0; k < REPEATS_NUM; k++)
        {
            tree = NULL;
            avl_tree = NULL;
            oh_hash_create(&oh_hash_table, 16, hash_func);
            ch_hash_create(&ch_hash_table, 16, hash_func);

            data_file = fopen(data_filename, "r");
            for (size_t j = 0; j < sizes[i]; j++)
            {
                getdelim(&data, &data_len, ' ', data_file);
                *strpbrk(data, " \n") = '\0';
                node = create_node(data);
                tree = insert(tree, node, cmp);
                avl_node = avl_create_node(data);
                avl_tree = avl_insert(avl_tree, avl_node, cmp);
                oh_hash_insert(&oh_hash_table, data);
                ch_hash_insert(&ch_hash_table, data);
            }

            char smb = rand() % 26 + 'a';

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            delete_by_first_letter(&tree, smb, cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            deletion_time[i][0] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            avl_delete_by_first_letter(&avl_tree, smb, cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            deletion_time[i][1] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            oh_hash_delete_by_smb(&oh_hash_table, smb);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            deletion_time[i][2] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            ch_hash_delete_by_smb(&ch_hash_table, smb);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            deletion_time[i][3] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            fclose(data_file);
            destroy_tree(tree);
            avl_destroy_tree(avl_tree);
            oh_hash_destroy(&oh_hash_table);
            ch_hash_destroy(&ch_hash_table);
        }
    }

    for (size_t i = 0; i < 5; i++)
        for (size_t j = 0; j < 4; j++)
            deletion_time[i][j] /= REPEATS_NUM;

    data = NULL;
    data_len = 0;

    for (size_t i = 0; i < 5; i++)
    {
        tree = NULL;
        avl_tree = NULL;
        oh_hash_create(&oh_hash_table, 16, hash_func);
        ch_hash_create(&ch_hash_table, 16, hash_func);

        snprintf(data_filename, 38, "./out/test_%zu_random.txt", sizes[i]);
        data_file = fopen(data_filename, "r");
        char **words = malloc(sizes[i] * sizeof(char *));
        for (size_t j = 0; j < sizes[i]; j++)
        {
            getdelim(&data, &data_len, ' ', data_file);
            *strpbrk(data, " \n") = '\0';
            node = create_node(data);
            tree = insert(tree, node, cmp);
            avl_node = avl_create_node(data);
            avl_tree = avl_insert(avl_tree, avl_node, cmp);
            oh_hash_insert(&oh_hash_table, data);
            ch_hash_insert(&ch_hash_table, data);
            words[j] = strdup(data);
        }
        fclose(data_file);

        for (size_t k = 0; k < sizes[i]; k++)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            search(tree, words[k], cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            search_time[i][0] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            avl_search(avl_tree, words[k], cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            printf("%llu\n", (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec));

            search_time[i][1] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            oh_hash_search(&oh_hash_table, words[k]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            search_time[i][2] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            ch_hash_search(&ch_hash_table, words[k]);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            search_time[i][3] += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        destroy_tree(tree);
        avl_destroy_tree(avl_tree);
        oh_hash_destroy(&oh_hash_table);
        ch_hash_destroy(&ch_hash_table);
        for (size_t k = 0; k < sizes[i]; k++)
            free(words[k]);
        free(words);
    }

    for (size_t i = 0; i < 5; i++)
        for (size_t j = 0; j < 4; j++)
            search_time[i][j] /= sizes[i];
    
    printf("Удаление по первому символу\n");
    char *separator = "+------+------------+------------+------------------+------------------+----------+----------+--------------------+--------------------+";
    printf("%s\n", separator);
    printf("| Слов |  BST (нс)  |  AVL (нс)  | Открытое         | Закрытое         |  BST     |  AVL     | Открытое           | Закрытое           |\n");
    printf("|      |            |            | хеширование (нс) | хеширование (нс) |  (байт)  |  (байт)  | хеширование (байт) | хеширование (байт) |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < 5; i++)
    {
        printf("| %4zu | %10llu | %10llu | %16llu | %16llu | %8zu | %8zu | %18zu | %18zu |\n", 
        sizes[i], deletion_time[i][0], deletion_time[i][1], deletion_time[i][2], deletion_time[i][3], 
        sizes[i] * (sizeof(tree_node_t) + wordsize + 1), sizes[i] * (sizeof(avl_tree_node_t) + wordsize + 1), 
        sizes[i] * (wordsize + 1 + sizeof(char *) + sizeof(hash_node_t)) + sizeof(oh_hash_table_t), sizes[i] * (wordsize + 1 + sizeof(char *)) + sizeof(ch_hash_table_t));
        printf("%s\n", separator);
    }

    printf("\n");

    printf("Поиск\n");
    printf("%s\n", separator);
    printf("| Слов |  BST (нс)  |  AVL (нс)  | Открытое         | Закрытое         |  BST     |  AVL     | Открытое           | Закрытое           |\n");
    printf("|      |            |            | хеширование (нс) | хеширование (нс) |  (байт)  |  (байт)  | хеширование (байт) | хеширование (байт) |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < 5; i++)
    {
        printf("| %4zu | %10llu | %10llu | %16llu | %16llu | %8zu | %8zu | %18zu | %18zu |\n", sizes[i], search_time[i][0], search_time[i][1], search_time[i][2], search_time[i][3], sizes[i] * (sizeof(tree_node_t) + wordsize + 1), sizes[i] * (sizeof(avl_tree_node_t) + wordsize + 1), 
        sizes[i] * (wordsize + 1 + sizeof(char *) + sizeof(hash_node_t)) + sizeof(oh_hash_table_t), sizes[i] * (wordsize + 1 + sizeof(char *)) + sizeof(ch_hash_table_t));
        printf("%s\n", separator);
    }

    return EXIT_SUCCESS;
}
