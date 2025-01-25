#include "profiling.h"

int cmp(void *word1, void *word2)
{
    return strcmp((char *)word1, (char *)word2);
}

void func(tree_node_t *tree, void *arg)
{
    return;
}

void file_delete_by_first_letter(FILE *dst, FILE *src, char smb)
{
    char *data = NULL;
    size_t data_len = 0;
    while (getdelim(&data, &data_len, ' ', src) != -1 && strlen(data) > 1)
    {
        *strpbrk(data, " \n") = '\0';
        if (data[0] != smb)
            fprintf(dst, "%s ", data);
        fprintf(dst, "\n");
    }   
}

int time_measurement(void)
{
    size_t sizes[5] = {1000, 2000, 5000, 10000, 20000};

    char *cmd = (char *)malloc(62);
    size_t wordsize = 5;

    char *balanced_filename = (char *)malloc(35);
    FILE *balanced_file;
    char *degenerated_filename = (char *)malloc(38);
    FILE *degenerated_file;

    long long balanced[5][3];
    memset(&balanced[0][0], 0, sizeof(long long) * 15);
    long long degenerated[5][3];
    memset(&degenerated[0][0], 0, sizeof(long long) * 15);

    char *data = (char *)malloc(wordsize);
    size_t data_len = 0;

    tree_node_t *tree = NULL;
    tree_node_t *node = NULL;

    struct timespec start, end;
    int64_t time_delta_ns = 0;

    for (size_t i = 0; i < 5; i++)
    {
        snprintf(cmd, 62, "python3 ./tools/gen_words.py --wordsnum %zu --wordsize %zu", sizes[i], wordsize);
        system(cmd);
    }

    for (size_t i = 0; i < 5; i++)
    {
        tree = NULL;
        snprintf(balanced_filename, 35, "./out/test_%zu_balanced.txt", sizes[i]);
        balanced_file = fopen(balanced_filename, "r");
        for (size_t j = 0; j < sizes[i]; j++)
        {
            getdelim(&data, &data_len, ' ', balanced_file);
            *strpbrk(data, " \n") = '\0';
            node = create_node(data);
            tree = insert(tree, node, cmp);
        }

        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            apply_in(tree, func, NULL);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        balanced[i][0] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;

        fclose(balanced_file);
        destroy_tree(tree);
    }

    time_delta_ns = 0;
    free(data);
    data = NULL;
    data_len = 0;
    for (size_t i = 0; i < 5; i++)
    {
        tree = NULL;
        snprintf(degenerated_filename, 38, "./out/test_%zu_degenerated.txt", sizes[i]);
        degenerated_file = fopen(degenerated_filename, "r");
        for (size_t j = 0; j < sizes[i]; j++)
        {
            getdelim(&data, &data_len, ' ', degenerated_file);
            *strpbrk(data, " \n") = '\0';
            node = create_node(data);
            tree = insert(tree, node, cmp);
        }

        for (size_t j = 0; j < REPEATS_NUM; j++)
        {
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            apply_in(tree, func, NULL);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
        }

        degenerated[i][0] = time_delta_ns / REPEATS_NUM;
        time_delta_ns = 0;

        fclose(degenerated_file);
        destroy_tree(tree);
    }

    time_delta_ns = 0;
    free(data);
    data = NULL;
    data_len = 0;
    for (size_t i = 0; i < 5; i++)
    {
        snprintf(balanced_filename, 35, "./out/test_%zu_balanced.txt", sizes[i]);
        for (size_t k = 0; k < REPEATS_NUM; k++)
        {
            tree = NULL;
            balanced_file = fopen(balanced_filename, "r");
            for (size_t j = 0; j < sizes[i]; j++)
            {
                getdelim(&data, &data_len, ' ', balanced_file);
                *strpbrk(data, " \n") = '\0';
                node = create_node(data);
                tree = insert(tree, node, cmp);
            }

            char smb = rand() % 26 + 'a';

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            delete_by_first_letter(&tree, smb, cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
            balanced[i][1] = time_delta_ns / REPEATS_NUM;
            time_delta_ns = 0;

            fseek(balanced_file, 0, SEEK_SET);
            FILE *tmp = tmpfile();
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            file_delete_by_first_letter(tmp, balanced_file, smb);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            fclose(tmp);

            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
            balanced[i][2] = time_delta_ns / REPEATS_NUM;
            time_delta_ns = 0;

            fclose(balanced_file);
            destroy_tree(tree);
        }
    }

    time_delta_ns = 0;
    free(data);
    data = NULL;
    data_len = 0;
    for (size_t i = 0; i < 5; i++)
    {
        snprintf(degenerated_filename, 38, "./out/test_%zu_degenerated.txt", sizes[i]);
        for (size_t k = 0; k < REPEATS_NUM; k++)
        {
            tree = NULL;
            degenerated_file = fopen(degenerated_filename, "r");
            for (size_t j = 0; j < sizes[i]; j++)
            {
                getdelim(&data, &data_len, ' ', degenerated_file);
                *strpbrk(data, " \n") = '\0';
                node = create_node(data);
                tree = insert(tree, node, cmp);
            }

            char smb = rand() % 26 + 'a';

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            delete_by_first_letter(&tree, smb, cmp);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);

            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
            degenerated[i][1] = time_delta_ns / REPEATS_NUM;
            time_delta_ns = 0;

            fseek(degenerated_file, 0, SEEK_SET);
            FILE *tmp = tmpfile();
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            file_delete_by_first_letter(tmp, degenerated_file, smb);
            clock_gettime(CLOCK_MONOTONIC_RAW, &end);
            fclose(tmp);

            time_delta_ns += (end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec);
            degenerated[i][2] = time_delta_ns / REPEATS_NUM;
            time_delta_ns = 0;

            fclose(degenerated_file);
            destroy_tree(tree);
        }
    }

    char *separator = "+--------------+--------------+-------------+-------------+";

    printf("Сбалансированное дерево\n");

    printf("%s\n", separator);
    printf("| Размер файла | Обход дерева | Удаление из | Удаление из |\n");
    printf("| (слов)       | (нс)         | дерева (нс) | файла (нс)  |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < 5; i++)
    {
        printf("| %-12zu | %-12lld | %-11lld | %-11lld |\n", sizes[i], balanced[i][0], balanced[i][1], balanced[i][2]);
        printf("%s\n", separator);
    }

    printf("Вырожденное дерево\n");

    printf("%s\n", separator);
    printf("| Размер файла | Обход дерева | Удаление из | Удаление из |\n");
    printf("| (слов)       | (нс)         | дерева (нс) | файла (нс)  |\n");
    printf("%s\n", separator);
    for (size_t i = 0; i < 5; i++)
    {
        printf("| %-12zu | %-12lld | %-11lld | %-11lld |\n", sizes[i], degenerated[i][0], degenerated[i][1], degenerated[i][2]);
        printf("%s\n", separator);
    }

    return EXIT_SUCCESS;
}
