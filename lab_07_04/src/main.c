#include "binary_tree.h"
#include "avl_tree.h"
#include "oh_hash_table.h"
#include "ch_hash_table.h"
#include "profiling.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define INIT_SIZE 17

static void print_menu(void);

static int word_cmp(void *word1, void *word2);

static size_t hash_func(char *key, size_t size);

int main(void)
{
    printf("Программа для демонстрации работы бинарного дерева поиска\n");
    printf("\n");

    int rc;
    int option;
    int is_running = 1;

    char *filename = NULL;
    size_t filename_len = 0;
    char *data = NULL;
    size_t data_len = 0;

    FILE *src_file = NULL;
    FILE *dst_file = NULL;

    tree_node_t *tree = NULL;
    tree_node_t *node = NULL;
    avl_tree_node_t *avl_tree = NULL;
    avl_tree_node_t *avl_node = NULL;

    oh_hash_table_t oh_hash_table;
    oh_hash_create(&oh_hash_table, 16, hash_func);
    ch_hash_table_t ch_hash_table;
    ch_hash_create(&ch_hash_table, 16, hash_func);

    char smb;
    char *seach_flag;
    hash_node_t *search_res;

    while (is_running)
    {
        print_menu();
        printf("Выберите опцию: ");
        rc = (scanf("%d", &option) == 1 && getchar() == '\n');

        if (rc == 1)
        {
            switch (option)
            {
            case 1:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                src_file = fopen(filename, "r");
                if (src_file == NULL)
                    printf("Невозможно открыть файл на чтение\n");
                else
                {
                    destroy_tree(tree);
                    avl_destroy_tree(avl_tree);
                    tree = NULL;
                    avl_tree = NULL;
                    while (getdelim(&data, &data_len, ' ', src_file) != -1 && strlen(data) > 1)
                    {
                        *strpbrk(data, " \n") = '\0';
                        node = create_node(data);
                        avl_node = avl_create_node(data);

                        tree = insert(tree, node, word_cmp);
                        avl_tree = avl_insert(avl_tree, avl_node, word_cmp);
                    }
                    fclose(src_file);
                }
                break;
            case 2:
                printf("Введите слово для добавления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                
                node = search(tree, data, word_cmp);
                avl_node = avl_search(avl_tree, data, word_cmp);

                if (node == NULL || avl_node == NULL)
                {
                    node = create_node(data);
                    tree = insert(tree, node, word_cmp);
                    avl_node = avl_create_node(data);
                    avl_tree = avl_insert(avl_tree, avl_node, word_cmp);
                    printf("Узел успешно добавлен\n");
                }
                else
                    printf("Слово %s уже есть в дереве\n", data);
                break;
            case 3:
                printf("Введите слово для поиска: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                node = search(tree, data, word_cmp);
                avl_node = avl_search(avl_tree, data, word_cmp);
                if (node == NULL && avl_node == NULL)
                    printf("Такого узла нет в дереве\n");
                else
                    printf("Слово %s есть в дереве\n", data);
                break;
            case 4:
                printf("Введите слово для удаления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                node = search(tree, data, word_cmp);
                avl_node = avl_search(avl_tree, data, word_cmp);
                if (node == NULL || avl_node == NULL)
                    printf("Такого узла нет в дереве\n");
                else
                {
                    tree = delete(tree, data, word_cmp);
                    avl_tree = avl_delete(avl_tree, data, word_cmp);
                    printf("Узел успешно удалён\n");
                }
                break;
            case 5:
                printf("Введите символ, по которому удалить слова\n");
                while (scanf("%c", &smb) != 1 || isspace(smb) || getchar() != '\n')
                {
                    printf("Невалидный символ.\n");
                    while (getchar() != '\n'); 
                }
                delete_by_first_letter(&tree, smb, word_cmp);
                avl_delete_by_first_letter(&avl_tree, smb, word_cmp);
                break;
            case 6:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                dst_file = fopen(filename, "w+");
                if (dst_file == NULL)
                    printf("Невозможно открыть файл на запись\n");
                else if (tree == NULL)
                    printf("Дерево пусто\n");
                else
                {
                    export_to_dot(dst_file, "BST", tree);
                    fclose(dst_file);
                }
                break;
            case 7:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                dst_file = fopen(filename, "w+");
                if (dst_file == NULL)
                    printf("Невозможно открыть файл на запись\n");
                else if (avl_tree == NULL)
                    printf("Дерево пусто\n");
                else
                {
                    avl_export_to_dot(dst_file, "AVL", avl_tree);
                    fclose(dst_file);
                }
                break;
            case 8:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                src_file = fopen(filename, "r");
                if (src_file == NULL)
                    printf("Невозможно открыть файл на чтение\n");
                else
                {
                    oh_hash_destroy(&oh_hash_table);
                    ch_hash_destroy(&ch_hash_table);
                    oh_hash_create(&oh_hash_table, INIT_SIZE, hash_func);
                    ch_hash_create(&ch_hash_table, INIT_SIZE, hash_func);
                    while (getdelim(&data, &data_len, ' ', src_file) != -1 && strlen(data) > 1)
                    {
                        *strpbrk(data, " \n") = '\0';
                        oh_hash_insert(&oh_hash_table, data);
                        ch_hash_insert(&ch_hash_table, data);
                    }
                    fclose(src_file);
                }
                break;
            case 9:
                printf("Введите слово для добавления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                
                oh_hash_insert(&oh_hash_table, data);
                ch_hash_insert(&ch_hash_table, data);
                printf("Слово успешно добавлено\n");
                break;
            case 10:
                printf("Введите слово для добавления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                
                search_res = oh_hash_search(&oh_hash_table, data);
                seach_flag = ch_hash_search(&ch_hash_table, data);
                if (search_res && seach_flag)
                    printf("Слово найдено\n");
                else
                    printf("Слово не найдено\n");
                break;
            case 11:
                printf("Введите слово для удаления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';

                rc = ch_hash_delete(&ch_hash_table, data);
                if (rc != 0)
                    printf("Такого слова нет в хэш-таблице\n");
                else
                    printf("Слово успешно удалёно\n");
                break;
            case 12:
                printf("Введите символ, по которому удалить слова\n");
                while (scanf("%c", &smb) != 1 || isspace(smb) || getchar() != '\n')
                {
                    printf("Невалидный символ.\n");
                    while (getchar() != '\n'); 
                }
                oh_hash_delete_by_smb(&oh_hash_table, smb);
                ch_hash_delete_by_smb(&ch_hash_table, smb);
                break;
            case 13:
                oh_hash_fprint(stdout, &oh_hash_table);
                break;
            case 14:
                ch_hash_fprint(stdout, &ch_hash_table);
                break;
            case 15:
                rc = time_measurement();
                if (rc != EXIT_SUCCESS)
                    printf("Ошибка выделения памяти\n");
                break;
            case 16:
                is_running = 0;
                break;
            default:
                printf("Введена невалидная опция\n");
                while (getchar() != '\n'); 
                break;
            }
        }
        else
        {
            printf("Введена невалидная опция\n");
            while (getchar() != '\n'); 
        }
        printf("\n");
    }

    free(filename);
    free(data);

    destroy_tree(tree);
    avl_destroy_tree(avl_tree);
    oh_hash_destroy(&oh_hash_table);
    ch_hash_destroy(&ch_hash_table);

    return EXIT_SUCCESS;
}

static void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Загрузить дерево из файла\n");
    printf("2 - Добавить элемент в дерево\n");
    printf("3 - Найти элемент в дереве\n");
    printf("4 - Удалить элемент из дерева\n");
    printf("5 - Удалить элементы, начинающиеся на заданную букву из дерева\n");
    printf("6 - Вывести бинарное дерево поиска\n");
    printf("7 - Вывести АВЛ дерево\n");
    printf("---\n");
    printf("8 - Загрузить хеш-таблицу из файла\n");
    printf("9 - Добавить элемент в хеш-таблицу\n");
    printf("10 - Найти элемент в хеш-таблице\n");
    printf("11 - Удалить элемент из хеш-таблицы\n");
    printf("12 - Удалить элементы, начинающиеся на заданную букву из хеш-таблицы\n");
    printf("13 - Вывести хеш-таблицу с открытым хешированием\n");
    printf("14 - Вывести хеш-таблицу с закрытым хешированием\n");
    printf("---\n");
    printf("15 - Анализ эффективности\n");
    printf("16 - Выход\n");
    printf("\n");
}

static int word_cmp(void *word1, void *word2)
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