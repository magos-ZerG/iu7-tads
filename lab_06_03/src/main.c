#include "binary_tree.h"
#include "profiling.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define _POSIX_C_SOURCE 200809L

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
ssize_t getdelim(char **lineptr, size_t *n, int delim, FILE *stream);

static void print_menu(void);

int word_cmp(void *word1, void *word2);

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

    char smb;

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
                    tree = NULL;
                    while (getdelim(&data, &data_len, ' ', src_file) != -1 && strlen(data) > 1)
                    {
                        *strpbrk(data, " \n") = '\0';
                        node = create_node(data);

                        tree = insert(tree, node, word_cmp);
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
                if (node == NULL)
                {
                    node = create_node(data);
                    tree = insert(tree, node, word_cmp);
                    printf("Узел успешно добавлен\n");
                }
                else
                    printf("Слово %s уже есть в дереве\n", node->data);
                break;
            case 3:
                printf("Введите слово для поиска: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                node = search(tree, data, word_cmp);
                if (node == NULL)
                    printf("Такого узла нет в дереве\n");
                else
                    printf("Слово %s есть в дереве\n", node->data);
                break;
            case 4:
                printf("Введите слово для удаления: ");
                while (getline(&data, &data_len, stdin) == -1 || strlen(data) <= 1 || strchr(data, ' ') != NULL)
                    printf("Невалидное слово.\n");
                *strpbrk(data, " \n") = '\0';
                node = search(tree, data, word_cmp);
                if (node == NULL)
                    printf("Такого узла нет в дереве\n");
                else
                {
                    tree = delete(tree, data, word_cmp);
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
                break;
            case 6:
                printf("Введите название файла: ");
                if (getline(&filename, &filename_len, stdin) == -1 || strlen(filename) <= 1)
                    printf("Невалидное имя файла.\n");
                filename[strlen(filename) - 1] = '\0';

                dst_file = fopen(filename, "w+");
                if (dst_file == NULL)
                    printf("Невозможно открыть файл на запись\n");
                else if (tree->data == NULL)
                    printf("Дерево пусто\n");
                else
                {
                    export_to_dot(dst_file, "BST", tree);
                    fclose(dst_file);
                }
                break;
            case 7:
                rc = time_measurement();
                if (rc != EXIT_SUCCESS)
                    printf("Ошибка выделения памяти\n");
                break;
            case 8:
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

    return EXIT_SUCCESS;
}

static void print_menu(void)
{
    printf("Меню:\n");
    printf("1 - Загрузить дерево из файла\n");
    printf("2 - Добавить элемент в дерево\n");
    printf("3 - Найти элемент в дереве\n");
    printf("4 - Удалить элемент из дерева\n");
    printf("5 - Удалить из дерева элементы, начинающиеся на заданную букву\n");
    printf("6 - Вывести дерево в файл\n");
    printf("7 - Анализ эффективности\n");
    printf("8 - Выход\n");
    printf("\n");
}

int word_cmp(void *word1, void *word2)
{
    return strcmp((char *)word1, (char *)word2);
}
